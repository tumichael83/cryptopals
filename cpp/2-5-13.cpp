#include "util/util.hpp"

#include <iostream>
#include <string>
#include <algorithm>

int uid = 0;
std::string key = "";
std::string IV = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

struct user_profile {
    std::string email;
    int uid;
    std::string role;
};

user_profile profile_for(std::string email) 
{
    // need to remove the = and &
    email.erase(remove(email.begin(), email.end(), '='), email.end());
    email.erase(remove(email.begin(), email.end(), '&'), email.end());

    return user_profile{email, uid++, "user"};
}

std::string to_json(user_profile& up) 
{
    return "{\n  email: '"+up.email+"'\n  uid:    "+std::to_string(up.uid)+"\n  role:  '"+up.role+"'\n}";
}

std::string to_cookie(user_profile& up)
{
    return "email="+up.email+"&uid="+std::to_string(up.uid)+"&role="+up.role;
}

std::string gen_rand_bytes(int n)
{
    std::string k;
    for (int i = 0; i < n; i++)
        k += (unsigned char)rand();
    return k;
}

std::string gen_rand_key() 
{
    if (key == "") key = gen_rand_bytes(BLOCK_SIZE);
    return key;
}

user_profile parse_cookie(std::string c) 
{
    auto amp = find(c.begin(), c.end(), '&');
    auto eql = find(c.begin(), c.end(), '=');
    auto email = c.substr(eql - c.begin()+1, amp-eql-1);

    amp = find(amp+1, c.end(), '&');
    eql = find(eql+1, c.end(), '=');
    auto my_uid = std::stoi(c.substr(eql - c.begin()+1, amp-eql-1));

    amp = c.end();
    eql = find(eql+1, c.end(), '=');
    auto role = c.substr(eql - c.begin()+1, amp-eql-1);

    return user_profile{email, my_uid, role};
}

std::string encrypt_profile(user_profile up)
{
    return encrypt_aes_128_ecb(pad(to_cookie(up), BLOCK_SIZE), gen_rand_key(), IV);
}

user_profile decrypt_profile(std::string ctext) 
{
    return parse_cookie(decrypt_aes_128_ecb(ctext, gen_rand_key(), IV));
}

std::string oracle(std::string email)
{
    return encrypt_profile(profile_for(email));
}

user_profile decrypt_oracle(std::string ctext)
{
    return decrypt_profile(ctext);
}

int main() {
    // attack code goes here!
    // what we want is to get a block that just encodes admin
    // then give an input where 'user' is the head of the last block

    // email= 6 char
    std::string attack_email = "frank.yangadmin\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b@gmail.com";
    auto ctext = oracle(attack_email);
    auto admin_block = ctext.substr(BLOCK_SIZE, BLOCK_SIZE);

    // email= 6 char
    // &uid=  5 char + 1 char for 1 dig uid
    // &role= 6 char
    // total: 18 char
    // need:  14 char email
    // 
    // fran@gmail.com

    // attack succeeded apart from padding
    auto base = oracle("fran@gmail.com");
    auto head = base.substr(0, 2*BLOCK_SIZE);

    auto fake = head + admin_block;

    auto up = decrypt_profile(fake);
    std::cout << to_json(up) << "\n";
}