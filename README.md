# Monitor de Academia com Minigame

Um monitor interativo para exercícios físicos que inclui um minigame divertido para tornar seu treino mais envolvente.

## Funcionalidades

- Monitoramento em tempo real da força aplicada
- Contador de repetições
- Minigame interativo com bolinhas
- Sistema de pontuação
- Timer de 30 segundos
- Controles de jogo (Iniciar, Pausar, Resetar)

## Requisitos

- Arduino compatível com comunicação serial
- Navegador moderno (Chrome ou Edge) com suporte à Web Serial API
- Sensor de força conectado ao Arduino

## Como Usar

1. Conecte o Arduino ao computador
2. Abra o arquivo `index.html` em um navegador compatível
3. Clique em "Conectar Arduino"
4. Inicie o minigame clicando em "Iniciar Jogo"
5. Use a força do exercício para mover a bolinha branca
6. Tente pegar as bolinhas douradas para marcar pontos

## Configuração do Arduino

O Arduino deve enviar dados no seguinte formato JSON:

```json
{
  "force": 0-100,  // Valor da força em porcentagem
  "reps": 0-N     // Número de repetições
}
```

## Tecnologias Utilizadas

- HTML5
- CSS3
- JavaScript
- Web Serial API
- Arduino

## Contribuição

Sinta-se à vontade para contribuir com o projeto através de Pull Requests ou reportando issues.

## Licença

MIT License