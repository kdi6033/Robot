
// 사물 인증서 (파일 이름: xxxxxxxxxx-certificate.pem.crt)
const char cert_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
GQvkm8H1EgR8/TaeoQ+Jhrt76quLRRtDot8/nNajN1n7NjuSf+DcsInUK+W1hXCQ
/TOYB744wWM6GE9qROizXlkSV6hK9sRKFSpXgKesRNQ6f4DFaaNr2Dr6AP+U8eDh
CMxTNbykYmafHlaaTY2cYXlts02qml89/8+QKp2UJ6fZerY6ojuZBUpnVrqaPDg2
3L3oMFT4izxL8tiaw5xaWpfBQo6c8mnh3oHdWFMDNhcIdzoQ4ja2CJIScBL06EEv
hzFRf0yqHKk6WYJP1j9QVXU6J/BtkVgLxJXAG5dtbMwhr2AA+Y6+en0ihGeN+Q==
-----END CERTIFICATE-----
)EOF";
// 사물 인증서 프라이빗 키 (파일 이름: xxxxxxxxxx-private.pem.key)
const char key_str[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
6oH7tyvQvMs+lqhKSeMb6nmIeSmc+Dqb4yJjwt3xhkM0y8jUc4pQdTcCgYEA04fZ
5RM38zB0iSSLuaBDkXDzSuYXQMlS7d/Noc2+7GkEVSDSz1cVDWRdqQe3BOYDDnY8
6B9Z1vynJFI+ElkxawmErnrWtn7k2ucKOWyEDzNoakww5hg4/roolTcCj90deIAU
GPb52eT5UI3klBQqxBNIpbgzM/AQ7VW5qkM8kFMCgYB1F7H1uwbYoqXJRMJ3wNT9
sU/FrDAzstOgdqLA/2EmGjR1/56KyzHqVpGXDA5QKnCtgaxUxIAL9L6DFBKiX1dz
NWnuEMh7lDCMQI+1I6RK5LtwE7fEHN6BuFIWiWN76ErQbgjzXc35tdo1Vs/X4Y2k
/MLxGbFWVfroNT7xDNc2EwKBgQC+X5eFCQTjw4o+Gzv9NfVZohP0Jpxq3EXce/1v
T+Iyllkl3EKeBDuYL+nzRFQ8/l1l2Jf8OKr8R6qNj875aHiHYquc9ETlPdLuB901
zSoVHCku8Z9YlhhFeHkpwbZ2j4QEfRCZXz2CEwcmrm03r+bFTK2nJ949MiOp1esh
Bx4srQKBgQCjb2WqglrEK/y5grA/AS9AfL6An0M4GZIc4c+K9rIL839rHdBUjUZD
zPpXX6MBkB8/ncc/w+WkSOh+Fs7PrIRZSxVVGTxQD8AblofO98slf7FGH8XT6/MO
o5X9ixjJc0U4DpeyrK0yTy6PgfXPtcoJef2b4EFDV9HFWoNuN2UniQ==
-----END RSA PRIVATE KEY-----

)EOF";
// Amazon Trust Services(ATS) 엔드포인트 CA 인증서 (서버인증 > "RSA 2048비트 키: Amazon Root CA 1" 다운로드)
const char ca_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
