from socket import *

def main():
    conect = ('localhost', 12000)
    client_socket = socket(AF_INET, SOCK_STREAM)
    client_socket.connect(conect)

    
    while True:
            # Recebe pergunta
            data = client_socket.recv(1024).decode('utf-8')
            if not data:  # se não vier nada, acabou
                break

            print(data)

            # Se for a mensagem final do servidor, só imprime
            if data.startswith("\nVocê acertou"):
                continue

            # Usuário responde
            answer = input("Sua resposta (A/B/C/D): ")
            client_socket.sendall(answer.strip().upper().encode('utf-8'))

    
    client_socket.close()

if __name__ == "__main__":
    main()
