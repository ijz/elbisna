# Elbisna
Elbisna is not Ansible

### Host file format
Host files are JSON files that contain hosts that you would want to run scripts on.
This is the same as inventory file in Ansible.
Example:
```
[
{
    "host": "web1.example.com",
    "ip": "123.45.67.89",
    "name": "web1",
    "role": "webserver"
},
{
    "host": "db1.example.com",
    "ip": "98.76.54.32",
    "name": "db1",
    "role": "mysql"
},
...
]
```
Either `host` or `ip` must be present in each host definition.
The rest of the key value pairs are arbitrary.

### Host file generation for cloud providers
1. AWS
_Dependency: python 2.7_
Run `scripts/aws_hosts.py` to scrape a given AWS account and generate a host file with all the instances.
All instance tag key value pairs will be written to the host file.

### epoll\_playground
```
make
./epoll_playgound test.fifo
cat examples/big.txt > test.fifo
```
