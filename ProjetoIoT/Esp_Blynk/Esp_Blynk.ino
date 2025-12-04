#define BLYNK_TEMPLATE_ID "TMPL2K6I_y8Oy"
#define BLYNK_TEMPLATE_NAME "Cadeado 2FA"
#define BLYNK_AUTH_TOKEN "2crxws8GkgJaQ93tF7Zfz4NoKe3JAb_h"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "REPJESUS";
char pass[] = "Dfppi123";

String tokenAtual = "";
bool aguardandoSenha = false; 
int tentativasRestantes = 0; // Contador de vidas

void setup() {
  Serial.begin(115200); 
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
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

void gerarNovoToken() {
  long numero = random(100000, 999999);
  tokenAtual = String(numero);
  
  aguardandoSenha = true;
  tentativasRestantes = 3; // Reseta as vidas para 3
  
  Blynk.virtualWrite(V6, "Digite a senha (3 chances)");
  
  Serial.print("C");
  Serial.println(tokenAtual); 
}

// --- V5: INPUT DE TEXTO ---
BLYNK_WRITE(V5) {
  String senhaRecebida = param.asString();

  if (!aguardandoSenha) {
     Blynk.virtualWrite(V6, "Nenhum movimento detectado");
     return;
  }

  // --- ACERTOU ---
  if (senhaRecebida == tokenAtual) {
    Blynk.virtualWrite(V6, "Validado! ABERTO.");
    Serial.write('A'); 
    aguardandoSenha = false; 
  } 
  // --- ERROU ---
  else {
    tentativasRestantes--; // Perde uma vida
    
    if (tentativasRestantes > 0) {
      // AINDA TEM CHANCES
      String msg = "Errado! Restam " + String(tentativasRestantes) + " chances";
      Blynk.virtualWrite(V6, msg);
      
      Serial.write('R'); // Manda Arduino mostrar "Tente Novamente"
    } 
    else {
      // ACABARAM AS CHANCES
      Blynk.virtualWrite(V6, "BLOQUEADO por 10s!");
      Serial.write('N'); // Manda Arduino bloquear por 10s
      aguardandoSenha = false; // Encerra o ciclo
    }
  }
}

// --- V8: BOTÃO TRANCAR ---
BLYNK_WRITE(V8) {
  int botao = param.asInt();
  if (botao == 1) { 
    Blynk.virtualWrite(V6, "Trancando Cofre...");
    Serial.write('F'); 
  }
}