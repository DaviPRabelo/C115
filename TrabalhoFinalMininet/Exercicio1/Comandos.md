# Exercício 1: Topologia Árvore

### a) Criar a topologia

Execute no terminal Linux para iniciar o Mininet com a topologia de 30 Mbps:

sudo mn --topo tree,3,5 --mac --link tc,bw=30


### b) Inspecionar informações

Execute no prompt :

# Listar os nós
mininet> nodes

# Listar informações simples
mininet> dump

# Inspecionar IP e MAC do host h1 
mininet> h1 ifconfig


### d) Testes de ping e tcpdump

1.  Abra os terminais para `h1` e `h2`(nas imagens foi usado h6 e h95):
    mininet> xterm h1 h2
    

2.  No terminal xterm do h1** que se abriu, inicie o `tcpdump`:
    
    h1# tcpdump -i h1-eth0 icmp
    

3.  No prompt principal do `mininet>`**, inicie o ping de `h2` para `h1`:
    
    mininet> h2 ping -c 4 h1


### e) Testes de iperf

#### Teste 1: Largura de Banda de 30 Mbps


1.  Abra os terminais (se ainda não estiverem abertos):
    mininet> xterm h1 h2
    

2.  **No terminal xterm do h1** (Servidor):
    h1# iperf -s -p 5555
    

3.  **No terminal xterm do h2** (Cliente), assumindo que o IP de h1 é `10.0.0.1`:
    h2# iperf -c 10.0.0.1 -p 5555 -t 20 -i 1
    

---

#### Teste 2: Largura de Banda de 40 Mbps

1.  Saia do Mininet atual:
    mininet> exit
    

2.  **No terminal Linux**, crie a nova topologia com 40 Mbps:
    
    $ sudo mn --topo tree,3,5 --mac --link tc,bw=40
    

3.  Repita os passos do `iperf` dentro do novo prompt `mininet>`:
    mininet> xterm h1 h2
    h1# iperf -s -p 5555
    h2# iperf -c 10.0.0.1 -p 5555 -t 20 -i 1
