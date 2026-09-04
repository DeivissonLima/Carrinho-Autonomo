// Sensor Ultrassônico
#define TRIG 2
#define ECHO 3

// Motor Esquerdo (Motor A)
#define ENA 6
#define IN1 7
#define IN2 8

// Motor Direito (Motor B)
#define ENB 5
#define IN3 9
#define IN4 10

// --- Configurações de Movimento ---
#define VEL_ESQ 200         // Velocidade motor esquerdo para ir RETO
#define VEL_DIR 230         // Velocidade motor direito para ir RETO
#define VEL_CURVA 255       // FORÇA MÁXIMA nas curvas para vencer o atrito do chão

// --- Configurações de Navegação ---
#define DIST_MINIMA 25      // cm - distância para detectar obstáculo
#define TEMPO_CURVA 900     // ms - TEMPO AUMENTADO! Ajuste até o carrinho fazer exatos 90°
#define TEMPO_AFASTAMENTO 800 // ms - tempo para ir para o lado e fugir do obstáculo
#define TEMPO_ULTRAPASSAR 1000 // ms - tempo para ir para frente e passar o obstáculo

void setup() {
  // Sensor Ultrassônico
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Motor Esquerdo
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Motor Direito
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  long distancia = medirDistancia();

  if (distancia > 0 && distancia <= DIST_MINIMA) {
    contornarObstaculo();
  } else {
    frente();
  }
}

long medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH, 30000); 
  long dist = duracao * 0.034 / 2;
  return dist;
}

void contornarObstaculo() {
  parar();
  delay(500); 

  // 1. Vira à ESQUERDA 90°
  virarEsquerda();
  delay(TEMPO_CURVA);
  parar(); delay(300);

  // 2. Avança para o LADO
  frente();
  delay(TEMPO_AFASTAMENTO);
  parar(); delay(300);

  // 3. Vira à DIREITA 90°
  virarDireita();
  delay(TEMPO_CURVA);
  parar(); delay(300);

  // 4. Avança para FRENTE (passando o obstáculo)
  frente();
  delay(TEMPO_ULTRAPASSAR);
  parar(); delay(300);

  // 5. Vira à DIREITA 90° (aponta de volta para a linha original)
  virarDireita();
  delay(TEMPO_CURVA);
  parar(); delay(300);

  // 6. VOLTA à linha original (Tempo igual ao passo 2 para fechar o retângulo)
  frente();
  delay(TEMPO_AFASTAMENTO);
  parar(); delay(300);

  // 7. Vira à ESQUERDA 90° para realinhar a frente
  virarEsquerda();
  delay(TEMPO_CURVA);
  parar(); delay(300);
}

// ---- Movimentos ----
void frente() {
  analogWrite(ENA, VEL_ESQ);
  analogWrite(ENB, VEL_DIR);
  digitalWrite(IN1, LOW);  
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW);  
}

void parar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);  
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  
  digitalWrite(IN4, LOW);
}

// Nas curvas, usamos VEL_CURVA (255) para garantir que os motores tenham força total
void virarEsquerda() {
  analogWrite(ENA, VEL_CURVA); 
  analogWrite(ENB, VEL_CURVA);
  digitalWrite(IN1, HIGH);  
  digitalWrite(IN2, LOW);  // Esq. para trás
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW);  // Dir. para frente
}

void virarDireita() {
  analogWrite(ENA, VEL_CURVA);
  analogWrite(ENB, VEL_CURVA);
  digitalWrite(IN1, LOW);  
  digitalWrite(IN2, HIGH); // Esq. para frente
  digitalWrite(IN3, LOW);  
  digitalWrite(IN4, HIGH); // Dir. para trás
}
