# Exercício 2: Topologia Customizada

Código Python

Primeiro, crie um arquivo chamado `Ex2Mininet.py` com o seguinte conteúdo:

from mininet.topo import Topo

class MyTopo(Topo):

    def build(self):
        h1 = self.addHost('h1')
        h2 = self.addHost('h2')
        h3 = self.addHost('h3')
        h4 = self.addHost('h4')
        h5 = self.addHost('h5')

        s1 = self.addSwitch('s1')
        s2 = self.addSwitch('s2')
        s3 = self.addSwitch('s3')

        self.addLink(h1, s1)
        self.addLink(h2, s1)
        self.addLink(s1, s2)

        self.addLink(h3, s2)
        self.addLink(s2, s3)

        self.addLink(h4, s3)
        self.addLink(h5, s3)

topos = { 'mytopo': ( lambda: MyTopo() ) }

### a) Criar a topologia


sudo mn --custom Ex2Mininet.py --topo=mytopo --mac --controller=remote


### b) Inspecionar informações

Execute no prompt `mininet>`:

# Inspecionar IP e MAC dos hosts
mininet> h1 ifconfig
mininet> h2 ifconfig
mininet> h3 ifconfig
mininet> h4 ifconfig
mininet> h5 ifconfig

# Inspecionar portas e MACs dos switchs
mininet> sh ovs-ofctl show s1
mininet> sh ovs-ofctl show s2
mininet> sh ovs-ofctl show s3

# Inspeciona endereços IP de todos
mininet> dump


### d) Teste de ping (falha inicial)

Como estamos usando um controlador remoto, o switch não sabe o que fazer com os pacotes.

mininet> h1 ping h4
mininet> h1 ping h2



### e) Apagar e criar regras baseadas em MAC


# Apagar regras antigas (boa prática)
mininet> sh ovs-ofctl del-flows s1
mininet> sh ovs-ofctl del-flows s2
mininet> sh ovs-ofctl del-flows s3

# Adicionar regras de ARP apenas para o switch 1 e 2 (para que os hosts se descubram)
mininet> sh ovs-ofctl add-flow s1 "arp,actions=flood"
mininet> sh ovs-ofctl add-flow s2 "arp,actions=flood"


# --- Regras para h1 <-> h3 (tráfego IP) ---

# Em s1: Pacote vindo de h1 (p1) para h3 (MAC ...03) -> enviar para s2 (p3)
mininet> sh ovs-ofctl add-flow s1 "ip,in_port=1,dl_dst=00:00:00:00:00:03,actions=output:3"
# Em s1: Pacote vindo de s2 (p3) para h1 (MAC ...01) -> enviar para h1 (p1)
mininet> sh ovs-ofctl add-flow s1 "ip,in_port=3,dl_dst=00:00:00:00:00:01,actions=output:1"

# Em s2: Pacote vindo de s1 (p1) para h3 (MAC ...03) -> enviar para h3 (p2)
mininet> sh ovs-ofctl add-flow s2 "ip,in_port=1,dl_dst=00:00:00:00:00:03,actions=output:2"
# Em s2: Pacote vindo de h3 (p2) para h1 (MAC ...01) -> enviar para s1 (p1)
mininet> sh ovs-ofctl add-flow s2 "ip,in_port=2,dl_dst=00:00:00:00:00:01,actions=output:1"


### f) Testes de ping para demonstrar

# Teste h1 -> h3 (DEVE FUNCIONAR)
mininet> h1 ping -c 3 h3

# Teste h1 -> h4 (DEVE FALHAR)
mininet> h1 ping -c 3 h5

(A falha do `h1 -> h4` demonstra que as regras foram implementadas corretamente, pois não criamos regras para `h4` nem para o switch `s3`).