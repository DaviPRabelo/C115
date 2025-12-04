#define BLYNK_TEMPLATE_ID "TMPL2K6I_y8Oy"
#define BLYNK_TEMPLATE_NAME "Cadeado 2FA"
#define BLYNK_AUTH_TOKEN "2crxws8GkgJaQ93tF7Zfz4NoKe3JAb_h"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <time.h> // Biblioteca nativa para pegar hora

char ssid[] = "REPJESUS";
char pass[] = "Dfppi123";

// Widget Terminal no V9
WidgetTerminal terminal(V9);

String tokenAtual = "";
bool aguardandoSenha = false; 
int tentativasRestantes = 0;

void setup() {
  Serial.begin(115200); 
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Configura hora: Fuso -3 (Brasil), sem horário de verão (0)
  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  
  terminal.println("----------------");
  terminal.println("Sistema Iniciado");
  terminal.println(pegarHora());
  terminal.flush(); // Envia para o app
}

void loop() {
  Blynk.run();
  
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'M') {
      gerarNovoToken();
    }
  }
}

// --- FUNÇÃO PARA PEGAR A HORA ATUAL ---
String pegarHora() {
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  
  // Formata para HH:MM:SS
  char buffer[10];
  sprintf(buffer, "%02d:%02d:%02d", p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
  return String(buffer);
}

// --- FUNÇÃO PARA ESCREVER NO HISTÓRICO ---
void logHistorico(String msg) {
  String hora = pegarHora();
  terminal.print("[");
  terminal.print(hora);
  terminal.print("] ");
  terminal.println(msg);
  terminal.flush(); // Força o envio imediato
}

void gerarNovoToken() {
  long numero = random(100000, 999999);
  tokenAtual = String(numero);
  
  aguardandoSenha = true;
  tentativasRestantes = 3; 
  
  Blynk.virtualWrite(V6, "Digite a senha...");
  logHistorico("Movimento detectado. Aguardando senha.");
  
  Serial.print("C");
  Serial.println(tokenAtual); 
}

// --- V5: INPUT SENHA ---
BLYNK_WRITE(V5) {
  String senhaRecebida = param.asString();

  if (!aguardandoSenha) {
     Blynk.virtualWrite(V6, "Nenhum movimento.");
     return;
  }

  // --- ACERTOU ---
  if (senhaRecebida == tokenAtual) {
    Blynk.virtualWrite(V6, "Validado! ABERTO.");
    logHistorico("SUCESSO: Cofre Aberto.");
    
    Serial.write('A'); 
    aguardandoSenha = false; 
  } 
  // --- ERROU ---
  else {
    tentativasRestantes--; 
    
    if (tentativasRestantes > 0) {
      String msg = "Senha Errada! Restam " + String(tentativasRestantes);
      Blynk.virtualWrite(V6, msg);
      logHistorico("FALHA: Senha Incorreta."); // Loga o erro
      
      Serial.write('R'); 
    } 
    else {
      Blynk.virtualWrite(V6, "BLOQUEADO!");
      logHistorico("ALERTA: Bloqueio Total (3 Erros).");
      
      Serial.write('N'); 
      aguardandoSenha = false; 
    }
  }
}

// --- V8: TRANCAR ---
BLYNK_WRITE(V8) {
  int botao = param.asInt();
  if (botao == 1) { 
    Blynk.virtualWrite(V6, "Trancando...");
    logHistorico("Manual: Cofre Trancado.");
    Serial.write('F'); 
  }
}