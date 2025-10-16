# Criação da topologia linear de 6 switchs com 25 Mbps de banda
sudo mn --topo linear,6 --mac --link tc,bw=25



# Linhas de comando para verificação de informações
verificação de informações específicas

h1 ifconfig

h2 ifconfig

s1 ifconfig

# Verificação de informações de todos os endereços
dump



# Executando ping entre diferentes nós
h1 ping h6

h6 ping h3

h5 ping h1

h2 ping h4


# Entrando nos terminais de cada nó
xterm h1 h2



# No terminal do h1, especificando que ele é o servidor na porta 5555
iperf -s -p 5555



# No terminal do h2, conectando ao servidor h1 dando relatório a cada segundo por 15 segundos
iperf -c 10.0.0.1 -p 5555 -t 15 -i 1

# Após terminar, digite exit nos terminais de h1 e h2 e depois exit no terminal do mininet

# Caso queira apagar após sair do mininet
sudo mn -c




