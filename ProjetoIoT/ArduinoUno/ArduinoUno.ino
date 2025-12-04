#include <LiquidCrystal.h>
#include <Servo.h>

// LCD: RS=12, E=11, D4=7, D5=6, D6=5, D7=4
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

Servo meuServo;
const int pinoServo = 9;
const int pinoPIR = 8;

unsigned long ultimoMovimento = 0;
const long intervaloBloqueio = 10000; 

// Variáveis de Estado
bool aguardandoToken = false;
bool cofreEstaAberto = false;
String tokenSalvo = ""; // Guardamos o token aqui para poder mostrar de novo se errar

void setup() {
  Serial.begin(115200); 
  
  lcd.begin(16, 2);
  lcd.print("Cofre IoT 2FA");
  
  meuServo.attach(pinoServo);
  meuServo.write(0); 
  pinMode(pinoPIR, INPUT);
  
  delay(2000);
  telaInicial();
}

void loop() {
  // --- 1. SENSOR PIR ---
  if (!aguardandoToken && !cofreEstaAberto && digitalRead(pinoPIR) == HIGH) {
    if (millis() - ultimoMovimento > intervaloBloqueio) {
      Serial.write('M'); 
      ultimoMovimento = millis();
      aguardandoToken = true;
      
      lcd.clear();
      lcd.print("Detectado!");
      lcd.setCursor(0, 1);
      lcd.print("Pedindo Token...");
    }
  }

  // --- 2. RECEBER COMANDOS DO ESP ---
  if (Serial.available()) {
    char comando = Serial.read();
    
    // --> RECEBEU TOKEN NOVO ('C')
    if (comando == 'C') { 
      tokenSalvo = Serial.readStringUntil('\n'); // Salva na memória
      tokenSalvo.trim();
      if (tokenSalvo.length() > 0) mostrarToken();
    }
    // --> ABRIR ('A')
    else if (comando == 'A') { 
      abrirCofre(); 
    }
    // --> TENTAR NOVAMENTE ('R') - Errou, mas tem chance
    else if (comando == 'R') { 
      tentativaFalha();
    }
    // --> BLOQUEIO TOTAL ('N') - Errou 3 vezes
    else if (comando == 'N') { 
      bloqueioTotal();
    }
    // --> FECHAR MANUAL ('F')
    else if (comando == 'F') { 
      fecharCofre();
    }
  }
}

// --- FUNÇÕES ---

void telaInicial() {
  lcd.clear();
  lcd.print("Seguranca Ativa");
  lcd.setCursor(0, 1);
  lcd.print("Trancado.");
}

void mostrarToken() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TOKEN DE ACESSO:");
  lcd.setCursor(5, 1); 
  lcd.print(tokenSalvo); 
}

void abrirCofre() {
  lcd.clear();
  lcd.print("Validado! ABERTO");
  meuServo.write(90); 
  
  aguardandoToken = false; 
  cofreEstaAberto = true; 
}

void tentativaFalha() {
  // Usuário errou, mas ainda tem tentativas
  lcd.clear();
  lcd.print("SENHA INCORRETA");
  lcd.setCursor(0, 1);
  lcd.print("Tente Novamente");
  
  delay(2000); // Mostra o erro por 2 segundos
  
  // IMPORTANTE: Volta a mostrar o token para ele tentar de novo
  mostrarToken(); 
}

void bloqueioTotal() {
  // Usuário errou 3 vezes
  lcd.clear();
  lcd.print("BLOQUEADO!");
  lcd.setCursor(0, 1);
  lcd.print("Espere 10s...");
  
  // O sistema congela aqui por 10 segundos (Requisito solicitado)
  delay(10000); 
  
  aguardandoToken = false; // Reseta o sistema
  telaInicial();
}

void fecharCofre() {
  lcd.clear();
  lcd.print("TRANCANDO...");
  meuServo.write(0); 
  delay(2000);
  
  cofreEstaAberto = false; 
  aguardandoToken = false; 
  ultimoMovimento = 0;     
  telaInicial();
}