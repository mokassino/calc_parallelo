#! /usr/bin/expect -f

set i [lindex $argv 0];

spawn docker exec -it $i ssh-copy-id -f -i /home/cpd2021/.ssh/id_rsa.pub master -o User=cpd2021 
expect "Are you sure you want to continue connecting (yes/no)? " {send "yes\r"}
expect "cpd2021@master's password: " {send "cpd2021\r"}

spawn docker exec -it $i ssh-copy-id -f -i /home/cpd2021/.ssh/id_rsa.pub node01 -o User=cpd2021 
expect "Are you sure you want to continue connecting (yes/no)? "
send "yes\r"
expect "cpd2021@node01's password: "
send "cpd2021\r"
