# Monitoramento de Adega – Vinheria Agnello

## 🔗 Links

- [Youtube]()
- [Wokwi]()

## 👥 Integrantes

- Bruce Li Yan Ting
- Felipe Rorato Costa
- Giovanna Andrade Parejas
- Laura Cardin Mirilli
- Matheus Pereira de Oliveira

## 📖 Descrição do Projeto

Este projeto é uma evolução do sistema de monitoramento de luminosidade desenvolvido anteriormente para a Vinheria Agnello.

Nesta versão, o sistema foi expandido para monitorar três fatores essenciais para a conservação dos vinhos: **luminosidade**, **temperatura** e **umidade**. As leituras são exibidas em um display LCD e o sistema emite alertas visuais e sonoros sempre que alguma condição sair da faixa ideal.

## 🎯 Objetivo

Monitorar as condições do ambiente de armazenamento de vinhos utilizando Arduino, sinalizando o estado através de LEDs, buzzer e display LCD, garantindo condições ideais para a conservação das bebidas.

## 🧪 Por que monitorar luz, temperatura e umidade?

A qualidade do vinho pode ser comprometida por três fatores ambientais:

**Luminosidade:** A exposição à luz, especialmente à radiação ultravioleta, provoca reações químicas que alteram o aroma, o sabor e aceleram a degradação da bebida. Vinhos brancos e espumantes são especialmente sensíveis.

**Temperatura:** A temperatura ideal de armazenamento fica entre 10°C e 15°C. Temperaturas acima desse limite aceleram o envelhecimento do vinho, enquanto temperaturas muito baixas podem comprometer sua estrutura.

**Umidade:** A umidade ideal fica entre 50% e 70%. Ambientes muito secos ressecam as rolhas, permitindo a entrada de ar e oxidando o vinho. Ambientes muito úmidos favorecem o aparecimento de fungos.

## 🔍 Funcionamento do Sistema

### Leituras e médias

Para garantir maior precisão, o sistema realiza **5 leituras consecutivas** de cada sensor e calcula a média antes de exibir os valores. As informações são atualizadas a cada **4 segundos**.

### Telas do display LCD

O display alterna automaticamente entre três telas a cada 4 segundos:

```text
Tela 1 → Luminosidade
Tela 2 → Temperatura
Tela 3 → Umidade
```

### Classificação da luminosidade

O sistema converte a leitura do LDR em lux e classifica o ambiente:

```text
0 a 30.000 lux      → Ambiente escuro (LED verde)
30.001 a 60.000 lux → Meia luz (LED amarelo + mensagem no display)
acima de 60.000 lux → Muito claro (LED vermelho + buzzer + mensagem no display)
```

### Classificação da temperatura

```text
Entre 10°C e 15°C → Temperatura OK (exibe valor no display)
Acima de 15°C     → Temp. Alta (LED amarelo + buzzer + exibe valor no display)
Abaixo de 10°C    → Temp. Baixa (LED amarelo + buzzer + exibe valor no display)
```

### Classificação da umidade

```text
Entre 50% e 70% → Umidade OK (exibe valor no display)
Acima de 70%    → Umidade Alta (LED vermelho + buzzer + exibe valor no display)
Abaixo de 50%   → Umidade Baixa (LED vermelho + buzzer + exibe valor no display)
```

### Prioridade dos LEDs

Como mais de uma condição pode estar fora do ideal ao mesmo tempo, o sistema segue uma ordem de prioridade:

```text
1º Vermelho → umidade fora do ideal ou luminosidade muito alta
2º Amarelo  → temperatura fora do ideal ou luminosidade em meia luz
3º Verde    → todas as condições dentro do ideal
```

O buzzer é acionado continuamente sempre que qualquer alerta vermelho ou amarelo estiver ativo.

## ⚙️ Componentes Utilizados

- 1 Arduino UNO R3
- 1 Placa de ensaio (Protoboard)
- 1 Fotorresistor (LDR) com sensor de luminosidade
- 1 Sensor DHT22 (temperatura e umidade)
- 1 Display LCD 16x2 com módulo I2C
- 1 LED Verde
- 1 LED Amarelo
- 1 LED Vermelho
- 1 Buzzer
- 1 Resistor de 10kΩ (para o LDR)
- 3 Resistores de 150Ω (para os LEDs)

## 🔌 Montagem do Circuito

### Alimentação

- Conecte o pino **5V** do Arduino à linha positiva (+) da protoboard
- Conecte o pino **GND** do Arduino à linha negativa (–) da protoboard

---

### Sensor LDR (divisor de tensão)

- Conecte uma perna do LDR ao **5V**
- Na mesma linha, conecte um resistor de **10kΩ** ao **GND** e um fio ao pino analógico **A0**
- Conecte a outra perna do LDR ao **GND**

---

### Sensor DHT22

- **VCC** → 5V do Arduino
- **GND** → GND do Arduino
- **DATA** → pino digital **2** do Arduino

---

### Display LCD I2C

- **VCC** → 5V
- **GND** → GND
- **SDA** → pino **A4**
- **SCL** → pino **A5**

---

### LEDs

| LED | Pino | Resistor |
|-----|------|----------|
| Verde | 10 | 150Ω |
| Amarelo | 9 | 150Ω |
| Vermelho | 8 | 150Ω |

Conecte o ânodo (perna longa) ao resistor e ao pino, e o cátodo (perna curta) ao GND.

---

### Buzzer

- Terminal positivo (+) → pino digital **1**
- Terminal negativo (–) → GND

---

## 💻 Bibliotecas Utilizadas

```text
- DHT sensor library (Adafruit)
- Adafruit Unified Sensor
- LiquidCrystal I2C (Frank de Brabander)
```

No Wokwi, adicione ao arquivo `libraries.txt`:

```text
DHT sensor library
Adafruit Unified Sensor
LiquidCrystal I2C
```

## ▶️ Como Executar o Projeto

1. Acesse o Wokwi
2. Importe o `diagram.json` com o circuito montado
3. Adicione as bibliotecas no `libraries.txt`
4. Cole o código no `sketch.ino`
5. Clique em **Play** para iniciar a simulação
6. Ajuste os valores do LDR e do DHT22 para observar os alertas

## ✅ Conclusão

O sistema desenvolvido expande o projeto anterior de monitoramento de luminosidade, adicionando o controle de temperatura e umidade.

A solução é simples, eficiente e exibe todas as informações em tempo real no display LCD, com alertas visuais e sonoros que garantem uma resposta rápida a qualquer condição inadequada no ambiente da adega.