'use strict';

// Protocolo HTTP compatível com o controle físico.
const DEMO_MODE = false;
const ESP32_BASE_URL = location.hostname === '192.168.10.103' ? '' : 'http://192.168.10.103';
const joystick = document.querySelector('#joystick');
const knob = document.querySelector('#knob');
const movement = document.querySelector('#movement');
const connection = document.querySelector('.connection');
const connectionText = document.querySelector('.connection-copy strong');
document.querySelector('.connection-copy span').textContent = '192.168.10.103';

function setConnectionStatus(connected) {
  connection.classList.toggle('disconnected', !connected);
  connectionText.textContent = connected ? 'CONECTADO' : 'DESCONECTADO';
  connection.setAttribute('aria-label', `${connected ? 'Conectado' : 'Desconectado'}${DEMO_MODE ? ' — modo demonstração' : ' — ESP32'}`);
}

async function checkConnection() {
  if (!navigator.onLine) return setConnectionStatus(false);
  if (DEMO_MODE) return setConnectionStatus(true);
  try {
    const response = await fetch(`${ESP32_BASE_URL}/`, {
      cache: 'no-store', signal: AbortSignal.timeout(2000),
    });
    setConnectionStatus(response.ok);
  } catch {
    setConnectionStatus(false);
  }
}
window.addEventListener('online', checkConnection);
window.addEventListener('offline', () => setConnectionStatus(false));
setConnectionStatus(false);
checkConnection();
if (!DEMO_MODE) setInterval(checkConnection, 5000);
const labels = { parar: 'PARADO', frente: 'FRENTE', re: 'RÉ', direita: 'DIREITA', esquerda: 'ESQUERDA' };
let currentCommand = 'parar';
let activePointer = null;
let requestQueue = Promise.resolve();
let pressedKeys = [];

function sendCommand(command) {
  if (DEMO_MODE) {
    console.log('[DEMO]', command);
    return;
  }
  // Ordem serial evita que um comando anterior chegue depois de /parar.
  requestQueue = requestQueue.then(async () => {
    try {
      const response = await fetch(`${ESP32_BASE_URL}/comando?acao=${encodeURIComponent(command)}`, {
        cache: 'no-store', signal: AbortSignal.timeout(1500),
      });
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      setConnectionStatus(true);
    } catch (error) {
      setConnectionStatus(false);
      console.error('Falha ao enviar comando ao ESP32:', command, error);
    }
  });
}

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

function moveKnob(dx, dy) {
  const radius = joystick.getBoundingClientRect().width * 0.18;
  const distance = Math.hypot(dx, dy);
  const scale = distance > radius ? radius / distance : 1;
  knob.style.transform = `translate(${dx * scale}px, ${dy * scale}px)`;
  const direction = distance < radius * 0.2 ? 'parar'
    : Math.abs(dx) > Math.abs(dy) ? (dx > 0 ? 'direita' : 'esquerda')
    : dy > 0 ? 're' : 'frente';
  setDirection(direction);
}

function movePointer(event) {
  const bounds = joystick.getBoundingClientRect();
  moveKnob(event.clientX - bounds.left - bounds.width / 2,
    event.clientY - bounds.top - bounds.height / 2);
}

function stop() {
  const pointer = activePointer;
  activePointer = null;
  pressedKeys = [];
  joystick.classList.remove('dragging');
  knob.style.transform = 'translate(0px, 0px)';
  setDirection('parar');
  if (pointer !== null && joystick.hasPointerCapture(pointer)) joystick.releasePointerCapture(pointer);
}

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
joystick.addEventListener('pointermove', event => {
  if (event.pointerId === activePointer) movePointer(event);
});
['pointerup', 'pointercancel', 'lostpointercapture'].forEach(type => {
  joystick.addEventListener(type, event => {
    if (event.pointerId === activePointer) stop();
  });
});
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
joystick.addEventListener('blur', stop);
window.addEventListener('blur', stop);
window.addEventListener('resize', stop);
window.addEventListener('pagehide', stop);
document.addEventListener('visibilitychange', () => { if (document.hidden) stop(); });

const fullscreenButton = document.querySelector('#fullscreen-button');
const fullscreenHint = document.querySelector('#fullscreen-hint');
let hintTimeout;
let automaticFullscreenAttempted = false;
const isStandalone = () => matchMedia('(display-mode: standalone)').matches || navigator.standalone === true;
const fullscreenElement = () => document.fullscreenElement || document.webkitFullscreenElement;

function showFullscreenHint(message) {
  clearTimeout(hintTimeout);
  fullscreenHint.textContent = message;
  fullscreenHint.hidden = false;
  hintTimeout = setTimeout(() => { fullscreenHint.hidden = true; }, 6500);
}

function updateFullscreenButton() {
  const expanded = Boolean(fullscreenElement()) || isStandalone();
  fullscreenButton.hidden = isStandalone();
  fullscreenButton.setAttribute('aria-pressed', String(expanded));
  const label = expanded ? 'Sair da tela cheia' : 'Entrar em tela cheia';
  fullscreenButton.setAttribute('aria-label', label);
  fullscreenButton.title = label;
  stop();
}

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
