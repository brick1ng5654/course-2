# -*- mode: ruby -*-
# vi: set ft=ruby :
ENV['PYTHONUTF8'] = '1'
ENV['LC_ALL'] = 'en.US.UTF-8'
ENV['LANG'] = "en_US.UTF-8"

Vagrant.configure("2") do |config|
  config.vm.box = "centos/7"

  config.vm.provider "virtualbox" do |vb|
	#Настройки VB
	vb.memory = "2048" # Выделение 2 ГБ ОЗУ
	vb.cpus = 2 # Выделение 2 ядер CPU
  end


  config.vm.network "private_network", ip: "192.168.33.10"

  config.vm.synced_folder "./", "/vagrant"

  config.vm.provision "shell", inline: <<-SHELL

  # Отключаем mirrorlist и заменяем baseurl на vault.centos.org
  sudo sed -i 's/mirrorlist=/#mirrorlist=/g' /etc/yum.repos.d/CentOS-*
  sudo sed -i 's|#baseurl=http://mirror.centos.org/centos|baseurl=http://vault.centos.org/centos|g' /etc/yum.repos.d/CentOS-*

  # Очищаем кеш YUM и удаляем старые метаданные
  sudo yum clean all
  sudo rm -rf /var/cache/yum

  # Импортируем GPG-ключ CentOS 7
  sudo rpm --import http://vault.centos.org/centos/7/os/x86_64/RPM-GPG-KEY-CentOS-7

  # Обновляем centos-release и создаём новый кеш
  sudo yum update -y centos-release
  sudo yum makecache

  # Устанавливаем EPEL-репозиторий и Ansible
  sudo yum install -y epel-release
  sudo yum install -y ansible
  SHELL

  config.vm.provision "ansible" do |ansible|
      ansible.playbook = "playbook.yml"
       
  end
end
