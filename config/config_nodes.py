#! /usr/bin/python3

import subprocess
import time

def main():
    
    print("begin nodes configuration")

    cmd = ["rm", "-f", "docker_ips"]
    subprocess.Popen(cmd)
    
    docker_i = []
    with open("docker_instances",'r') as f:
        lines = f.readlines()
        for line in lines:
#            print(line[0:12] + '\n')
            docker_i.append(line[0:12])


#    print(docker_i)

    ipaddr = []
    for i in docker_i:
        pipe = subprocess.PIPE

        # config each node
        cmd = ["/bin/bash", "foreach_node_config1.sh", i]
        child = subprocess.Popen(cmd, stdout=pipe)
        subprocess.Popen.wait(child)

        tmp = child.stdout.read().decode()
        ipaddr.append(tmp[0:len(tmp)-3])

    with open("docker_ips",'a') as f:
        for ip in ipaddr:
            f.write(ip + '\n')
        
    print(docker_i)
    if ( len(ipaddr) == 2):
        for i in docker_i:
                pipe = subprocess.PIPE
                cmd = ["/bin/bash", "foreach_node_config2.sh",i]
                for ip in ipaddr:
                    cmd.append(ip)
                child = subprocess.Popen(cmd)
                time.sleep(1)


if __name__ == "__main__" :
    main()

