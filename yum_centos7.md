# yum repository

server 管理 rpm 包。将 rpm 放入发布目录，createrepo 为 rpm 生成 meta 信息。内网可以使用离线版本；具体操作参考reference。

client 使用yum 获取包，从  /etc/yum.repos.d 遍历远端，本地一般缓存在 /var/cache/yum。

## yum server

```bash
# Yum typically delivers files over either FTP or HTTP.
# Configure Network Access.
sudo yum install httpd

# enbale firewall for http

# Create Yum Local Repository
# This software bundles several .rpm files together into a repomd repository. 
sudo yum install createrepo

# Create a Directory to Store the Repositories
mkdir –p /var/www/html/repos/{base,centosplus,extras,updates}

# Synchronize HTTP Repositories
# 内网环境可以下载iso 文件，copy rpm 包，参考 reference
sudo reposync -g -l -d -m --repoid=base --newest-only --download-metadata --download_path=/var/www/html/repos/
sudo reposync -g -l -d -m --repoid=centosplus --newest-only --download-metadata --download_path=/var/www/html/repos/
sudo reposync -g -l -d -m --repoid=extras --newest-only --download-metadata --download_path=/var/www/html/repos/
sudo reposync -g -l -d -m --repoid=updates --newest-only --download-metadata --download_path=/var/www/html/repos/

# Create the New Repository
createrepo /var/www/html
```



* __httpd directory__

  /var/www/html 是web 服务器默认根文件。

* __enable firewall__ 

```bash
# 防火墙确保http 80端口可用 man firewall-cmd
systemctl status firewalld # or firewall-cmd –-state 查看防火墙是否启用
yum install firewalld firewalld-config # 安装防火墙
# 查看、开启、关闭端口
firewall-cmd –permanent –list-port
firewall-cmd –zone=public –add-port=80/tcp –permanent
firewall-cmd –zone=public –remove-port=80/tcp –permanent
firewall-cmd –zone=public –add-port=4400-4600/udp –permanent # 批量添加

# 管理服务（example for http service，belows will open port 80）
firewall-cmd --zone=public --permanent --add-service=http
firewall-cmd --zone=public --permanent --add-service=https
firewall-cmd --reload

# firwall 可用于转发，比如端口转发
firewall-cmd --zone=external --add-masquerade # 转发前首先打开ip 地址伪装
firewall-cmd --zone=external --add-forward-port=22:porto=tcp::toport=3573:toaddr=192.1.1.2
```



## yum client

yum 全称 yellow dog updater, Modifier，基于 rpm 的包管理器，自动管理、安装依赖。

* yum search
* yum install
* yum list
* yum remove
* yum clean

### 配置 yum 源

```bash
vim /etc/yum.repos.d/base.repo
[base] #仓库的名字 
name=base #描述 
baseurl=file:///media #服务器软件包所在的绝对路径 (ftp:// or http:// or file:// )
gpgcheck=0 #对包是否进行检测（1检测，0不检测） 
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-redhat-release #gpgkey的绝对路径
enable=1 #是否启用（1启用，0关闭）

# 执行 yum 源更新命令
yum clean all
yum makecache
yum update
```

## reference

[搭建内网 yum server](https://www.cnblogs.com/zhangwencheng/p/16738717.html)

[**How to Set Up Local Yum Repositories on CentOS 7**](https://phoenixnap.com/kb/create-local-yum-repository-centos)