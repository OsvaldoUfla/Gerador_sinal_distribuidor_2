/*
 * Simulador de Sinal de Rotação (RPM) - Opala 4 Cilindros
 * Hardware: ESP32
 * Saída: Onda quadrada (Duty Cycle 50%)
 */

#define PINO_SAIDA 25  // Pino onde sairá o sinal (GPIO 25)

// Configurações do PWM (LEDC)
// O 'canal_pwm' não é mais usado na configuração inicial
// O ESP32 atribui o canal automaticamente.
// const int canal_pwm = 0; // Removido ou não utilizado no setup()
const int resolucao_pwm = 8; // 8 bits (0-255)
const int pulsos_por_volta = 2; // 4 Cilindros = 2 pulsos

int rpm_atual = 1000; // Começa em 1000 RPM

void setup() {
  Serial.begin(115200);

  // A função ledcAttach() agora configura o pino, frequência e resolução
  // em uma única chamada, substituindo ledcSetup() e ledcAttachPin().
  // Nota: A frequência inicial será atualizada logo abaixo
  
  // ledcSetup(canal_pwm, 33, resolucao_pwm); // REMOVIDA
  // ledcAttachPin(PINO_SAIDA, canal_pwm);    // REMOVIDA
  
  // Nova chamada para associar o pino à configuração inicial de PWM:
  // ledcAttach(pin, freq, resolution)
  // Usamos uma frequência inicial baixa, que será atualizada
  ledcAttach(PINO_SAIDA, 33, resolucao_pwm); 
  
  atualizarFrequencia(rpm_atual);
  
  Serial.println("--- Gerador de Sinais RPM (4 Cilindros) ---");
  Serial.println("Digite um valor de RPM no monitor serial (ex: 3000) e dê Enter.");
}

void loop() {
  // Verifica se você digitou um novo RPM no computador
  if (Serial.available() > 0) {
    int novo_rpm = Serial.parseInt();
    
    // Filtro simples para evitar valor zero ou lixo de leitura
    if (novo_rpm > 0) {
      rpm_atual = novo_rpm;
      atualizarFrequencia(rpm_atual);
    }
  }
  
  // O loop fica livre! O sinal é gerado pelo hardware em segundo plano.
  delay(100);
}

void atualizarFrequencia(int rpm) {
  // FÓRMULA: Frequencia = (RPM * Pulsos_por_volta) / 60 segundos
  double frequencia = (double)(rpm * pulsos_por_volta) / 60.0;
  
  // *IMPORTANTE: NA VERSÃO 3.0+, ledcSetup() foi removida.*
  // A frequência é atualizada com a nova função ledcAttach(), 
  // usando o PINO em vez do CANAL como primeiro argumento.
  
  // ledcSetup(canal_pwm, frequencia, resolucao_pwm); // REMOVIDA
  ledcAttach(PINO_SAIDA, frequencia, resolucao_pwm); 
  
  // ledcWrite(canal_pwm, 128); // Na versão 3.0+, ledcWrite() usa o PINO
  // Mantém Duty Cycle em 50% (128 de 255) para simular onda quadrada perfeita
  ledcWrite(PINO_SAIDA, 128); 
  
  Serial.print("RPM: ");
  Serial.print(rpm);
  Serial.print(" | Frequencia: ");
  Serial.print(frequencia);
  Serial.println(" Hz");
}