from socket import *

portaServidor = 12000

def start_server():
    server_socket = socket(AF_INET, SOCK_STREAM)
    server_socket.bind(('localhost', portaServidor))
    server_socket.listen(1)  
    print("Servidor iniciado e aguardando conexões...")

    conn, addr = server_socket.accept()
    print(f"Conexão de {addr} estabelecida!")

    perguntas = [
        {
            "pergunta": "Qual é a fórmula química da água?",
            "opcoes": ["A) CO2", "B) H2O", "C) O2", "D) NaCl"],
            "resposta": "B"
        },
        {
            "pergunta": "Em que ano o homem pisou na Lua pela primeira vez?",
            "opcoes": ["A) 1965", "B) 1969", "C) 1971", "D) 1975"],
            "resposta": "B"
        }
    ]
   
    respostas_certas = 0
    resultados = []

    for p in perguntas:
            # Envia a pergunta
            texto_pergunta = f"{p['pergunta']}\n" + "\n".join(p['opcoes']) + "\n"
            conn.sendall(texto_pergunta.encode())

            # Recebe a resposta
            resposta = conn.recv(1024).decode().strip()

            # Confere resultado
            if resposta.upper() == p['resposta']:
                respostas_certas += 1
                resultados.append(f"Pergunta: {p['pergunta']} - Sua resposta: {resposta} - Correta!")
            else:
                resultados.append(
                    f"Pergunta: {p['pergunta']} - Sua resposta: {resposta} - Incorreta! Resposta correta: {p['resposta']}"
                )

    msg_final = f"\nVocê acertou {respostas_certas} de {len(perguntas)} perguntas.\n"
    conn.sendall(msg_final.encode())
    for resultado in resultados:
            conn.sendall((resultado + "\n").encode())

    conn.close()
    server_socket.close()

if __name__ == "__main__":
    start_server()
