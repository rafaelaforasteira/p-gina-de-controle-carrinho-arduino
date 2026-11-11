'use strict';

/* GUIA DO CONTROLE
 * HTML monta a página; CSS desenha; JavaScript interpreta o toque.
 * Caminho: dedo -> movePointer -> moveKnob -> setDirection -> sendCommand.
 * O ESP32 recebe /comando?acao=frente e chama a função dos motores.
 * As funções estão em inglês, mas os comandos enviados são em português.
 */

// Protocolo HTTP compatível com o controle físico.
// false: envia comandos reais. true: apenas registra no console.
const DEMO_MODE = false;
// Página aberta no IP privado do ESP32: usa o próprio servidor (endereço vazio).
// Fora dele: tenta o último IP conhecido. A expressão verifica faixas de IP privado.
const ESP32_BASE_URL = location.protocol === 'http:' && /^(192\.168\.|10\.|172\.(1[6-9]|2\d|3[01])\.)/.test(location.hostname) ? '' : 'http://192.168.10.108';
// querySelector procura no HTML os elementos que serão atualizados.
const joystick = document.querySelector('#joystick');
const knob = document.querySelector('#knob');
const movement = document.querySelector('#movement');
const connection = document.querySelector('.connection');
const connectionText = document.querySelector('.connection-copy strong');
document.querySelector('.connection-copy span').textContent = ESP32_BASE_URL ? new URL(ESP32_BASE_URL).hostname : location.hostname;

// Atualiza texto, cor e descrição acessível. A pulsação é feita pelo CSS.
function setConnectionStatus(connected) {
  connection.classList.toggle('disconnected', !connected);
  connectionText.textContent = connected ? 'CONECTADO' : 'DESCONECTADO';
  connection.setAttribute('aria-label', `${connected ? 'Conectado' : 'Desconectado'}${DEMO_MODE ? ' — modo demonstração' : ' — ESP32'}`);
}

// async/await permite aguardar a resposta HTTP sem travar a interface.
// /status só verifica se o servidor responde; não lê luminosidade ou velocidade.
async function checkConnection() {
  if (!navigator.onLine) return setConnectionStatus(false);
  if (DEMO_MODE) return setConnectionStatus(true);
  try {
    const response = await fetch(`${ESP32_BASE_URL}/status`, {
      cache: 'no-store', signal: AbortSignal.timeout(2000),
    });
    setConnectionStatus(response.ok);
  } catch {
    setConnectionStatus(false);
  }
}
// Eventos do navegador + consulta a cada 5 segundos mantêm o indicador atualizado.
window.addEventListener('online', checkConnection);
window.addEventListener('offline', () => setConnectionStatus(false));
setConnectionStatus(false);
checkConnection();
if (!DEMO_MODE) setInterval(checkConnection, 5000);
const labels = { parar: 'PARADO', frente: 'FRENTE', re: 'RÉ', direita: 'DIREITA', esquerda: 'ESQUERDA' };
// Estado do controle: direção atual, dedo ativo e requisição pendente.
let currentCommand = 'parar';
let activePointer = null;
let commandInFlight = false;
let pendingCommand = null;
let pressedKeys = [];

// Envia GET /comando?acao=... para o ESP32.
// Só uma requisição fica em andamento; a próxima é substituída pela mais recente.
// Assim não acumulamos movimentos antigos enquanto esperamos a rede responder.
async function sendCommand(command) {
  if (DEMO_MODE) {
    console.log('[DEMO]', command);
    return;
  }
  // Uma requisição por vez; só guarda o comando mais recente.
  pendingCommand = command;
  if (commandInFlight) return;
  commandInFlight = true;
  try {
    while (pendingCommand !== null) {
      const next = pendingCommand;
      pendingCommand = null;
      try {
// fetch faz o pedido HTTP. encodeURIComponent prepara o texto para a URL.
        // no-store evita cache; timeout limita quanto esperamos pela resposta.
        const response = await fetch(`${ESP32_BASE_URL}/comando?acao=${encodeURIComponent(next)}`, {
          cache: 'no-store', signal: AbortSignal.timeout(800),
        });
        if (!response.ok) throw new Error(`HTTP ${response.status}`);
        setConnectionStatus(true);
      } catch (error) {
        setConnectionStatus(false);
        console.error('Falha ao enviar comando ao ESP32:', next, error);
        // Interrompe a interação e tenta enviar parar uma vez.
        if (next !== 'parar') stop();
      }
    }
  } finally {
    commandInFlight = false;
  }
}

// Só envia quando a direção muda. Arrastar na mesma direção não repete pedidos.
// dataset e classes conectam o estado do JavaScript aos efeitos visuais do CSS.
function setDirection(command) {
  if (command === currentCommand) return;
  currentCommand = command;
  joystick.dataset.direction = command;
  movement.value = labels[command];
  movement.parentElement.classList.toggle('moving', command !== 'parar');
  document.querySelectorAll('[data-highlight]').forEach(element => {
    element.classList.toggle('active', element.dataset.highlight === command);
  });
  sendCommand(command);
}

// dx: distância horizontal do dedo ao centro; dy: distância vertical (em pixels).
// Na tela, x cresce para a direita e y cresce para baixo.
function moveKnob(dx, dy) {
  // O deslocamento máximo é 18% da largura do joystick.
  const radius = joystick.getBoundingClientRect().width * 0.18;
  // Pitágoras: hypot(dx, dy) = raiz de (dx² + dy²).
  const distance = Math.hypot(dx, dy);
  // Se passou do limite, reduz os dois eixos na mesma proporção.
  const scale = distance > radius ? radius / distance : 1;
  knob.style.transform = `translate(${dx * scale}px, ${dy * scale}px)`;
  // Zona morta: os primeiros 20% do deslocamento permitido significam parado.
  // abs ignora o sinal para comparar qual eixo teve maior deslocamento.
  let direction;
  if (distance < radius * 0.2) {
    direction = 'parar';
  } else if (Math.abs(dx) > Math.abs(dy)) {
    if (dx > 0) {
      direction = 'direita';
    } else {
      direction = 'esquerda';
    }
  } else {
    if (dy > 0) {
      direction = 're';
    } else {
      direction = 'frente';
    }
  }
  setDirection(direction);
}

// Converte a posição do toque na tela em deslocamento em relação ao centro.
function movePointer(event) {
  const bounds = joystick.getBoundingClientRect();
  moveKnob(event.clientX - bounds.left - bounds.width / 2,
    event.clientY - bounds.top - bounds.height / 2);
}

// Centraliza o desenho e muda a direção para parar.
// Tenta enviar a parada; uma falha de rede ainda pode impedir sua entrega.
function stop() {
  const pointer = activePointer;
  activePointer = null;
  pressedKeys = [];
  joystick.classList.remove('dragging');
  knob.style.transform = 'translate(0px, 0px)';
  setDirection('parar');
  if (pointer !== null && joystick.hasPointerCapture(pointer)) joystick.releasePointerCapture(pointer);
}

// Pointer Events funcionam com mouse, dedo e caneta.
// pointerdown começa o gesto; a captura mantém os eventos mesmo fora do círculo.
joystick.addEventListener('pointerdown', event => {
  if (activePointer !== null || (event.pointerType === 'mouse' && event.button !== 0)) return;
  event.preventDefault();
  pressedKeys = [];
  activePointer = event.pointerId;
  joystick.setPointerCapture(event.pointerId);
  joystick.focus({ preventScroll: true });
  joystick.classList.add('dragging');
  movePointer(event);
});
// Só acompanha o dedo que iniciou o gesto (ignora um segundo toque).
joystick.addEventListener('pointermove', event => {
  if (event.pointerId === activePointer) movePointer(event);
});
// Soltar, cancelar ou perder a captura encerra o movimento.
['pointerup', 'pointercancel', 'lostpointercapture'].forEach(type => {
  joystick.addEventListener(type, event => {
    if (event.pointerId === activePointer) stop();
  });
});
// Teclado reaproveita a mesma lógica do toque: cada seta representa um vetor.
// Se várias teclas estiverem pressionadas, vale a última; não há diagonais.
const keyVectors = { ArrowUp: [0, -1], ArrowDown: [0, 1], ArrowLeft: [-1, 0], ArrowRight: [1, 0] };
function updateKeyboard() {
  const vector = keyVectors[pressedKeys.at(-1)];
  if (!vector) return stop();
  const radius = joystick.getBoundingClientRect().width * 0.18;
  moveKnob(vector[0] * radius, vector[1] * radius);
}
joystick.addEventListener('keydown', event => {
  if (!keyVectors[event.key] || activePointer !== null) return;
  event.preventDefault();
  if (!pressedKeys.includes(event.key)) pressedKeys.push(event.key);
  updateKeyboard();
});
joystick.addEventListener('keyup', event => {
  if (!keyVectors[event.key] || activePointer !== null) return;
  event.preventDefault();
  pressedKeys = pressedKeys.filter(key => key !== event.key);
  updateKeyboard();
});
// Perda de foco, troca de aba e redimensionamento também tentam parar.
joystick.addEventListener('blur', stop);
window.addEventListener('blur', stop);
window.addEventListener('resize', stop);
window.addEventListener('pagehide', stop);
document.addEventListener('visibilitychange', () => { if (document.hidden) stop(); });

// Parte independente do controle dos motores: botão e avisos de tela cheia.
// Prefixo webkit atende navegadores que usam esse nome para a mesma API.
const fullscreenButton = document.querySelector('#fullscreen-button');
const fullscreenHint = document.querySelector('#fullscreen-hint');
let hintTimeout;
let automaticFullscreenAttempted = false;
const isStandalone = () => matchMedia('(display-mode: standalone)').matches || navigator.standalone === true;
const fullscreenElement = () => document.fullscreenElement || document.webkitFullscreenElement;

// Exibe orientação temporária quando a tela cheia não está disponível.
function showFullscreenHint(message) {
  clearTimeout(hintTimeout);
  fullscreenHint.textContent = message;
  fullscreenHint.hidden = false;
  hintTimeout = setTimeout(() => { fullscreenHint.hidden = true; }, 6500);
}

// Sincroniza o ícone e interrompe o movimento ao mudar a tela.
function updateFullscreenButton() {
  const expanded = Boolean(fullscreenElement()) || isStandalone();
  fullscreenButton.hidden = isStandalone();
  fullscreenButton.setAttribute('aria-pressed', String(expanded));
  const label = expanded ? 'Sair da tela cheia' : 'Entrar em tela cheia';
  fullscreenButton.setAttribute('aria-label', label);
  fullscreenButton.title = label;
  stop();
}

// O navegador só permite solicitar tela cheia após interação do usuário.
async function enterFullscreen(showHint = true) {
  if (fullscreenElement() || isStandalone()) return;
  const root = document.documentElement;
  try {
    if (root.requestFullscreen && document.fullscreenEnabled) {
      await root.requestFullscreen();
    } else if (root.webkitRequestFullscreen) {
      await root.webkitRequestFullscreen();
    } else {
      throw new Error('Fullscreen indisponível');
    }
  } catch {
    if (showHint) showFullscreenHint(/iPhone|iPad|iPod/.test(navigator.userAgent)
      ? 'Para abrir sem as barras: no Safari, toque em Compartilhar → Adicionar à Tela de Início e abra pelo ícone.'
      : 'Tela cheia indisponível neste navegador. Use a opção Adicionar à tela inicial ou abra em outro navegador.');
  }
}

fullscreenButton.addEventListener('click', async () => {
  automaticFullscreenAttempted = true;
  stop();
  if (fullscreenElement()) {
    try {
      if (document.exitFullscreen) await document.exitFullscreen();
      else if (document.webkitExitFullscreen) await document.webkitExitFullscreen();
    } catch { showFullscreenHint('Use o comando de voltar do navegador para sair da tela cheia.'); }
  } else await enterFullscreen();
});

// O navegador exige um gesto. Não altera a tela durante uma manobra.
document.addEventListener('click', event => {
  if (automaticFullscreenAttempted || !matchMedia('(pointer: coarse)').matches
    || event.target.closest('#joystick, #fullscreen-button')) return;
  automaticFullscreenAttempted = true;
  enterFullscreen(false);
});
document.addEventListener('fullscreenchange', updateFullscreenButton);
document.addEventListener('webkitfullscreenchange', updateFullscreenButton);
updateFullscreenButton();
