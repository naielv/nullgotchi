$(document).ready(function() {
  var status = "SLEEPING", mood = "OK", ticks = 0;
  var hygiene = 100, rest = 50, food = 100, entertainment = 80;

  function updateMood() {
    var minVal = Math.min(
      Math.min(hygiene, rest),
      Math.min(food, entertainment)
    );

    if(status === "SLEEPING") {
      mood = "DREAMING";
    } else if(minVal === 0) {
      mood = "LEFT";
      status = "DEAD";
    } else if((hygiene === minVal) && (hygiene < 50)) {
      mood = "DIRTY";
    } else if((rest === minVal) && (rest < 50)) {
      mood = "TIRED";
    } else if((food === minVal) && (food < 50)) {
      mood = "HUNGRY";
    } else if((entertainment === minVal) && (entertainment < 50)) {
      mood = "BORED";
    } else if(minVal > 80) {
      mood = "VERY HAPPY";
    } else if(minVal > 65) {
      mood = "HAPPY";
    } else {
      mood = "GRUMPY";
    }
  }

  function changeBy(value, delta) {
    value += delta;
    value = Math.max(0, value);
    value = Math.min(100, value);
    return value;
  }

  function updateUI() {
    $('#status').text(status);
    $('#mood').text(mood);
    $('#ticks').text(ticks);

    $('#hygiene').text(hygiene);
    $('#rest').text(rest);
    $('#food').text(food);
    $('#entertainment').text(entertainment);
  }

  function advanceState() {
    switch(status) {
      case "SLEEPING":
        rest = changeBy(rest, 3);
        hygiene = changeBy(hygiene, -1);
        food = changeBy(food, -2);
        if(rest == 100) {
          status = "NEUTRAL";
        }
        break;
      case "NEUTRAL":
        rest = changeBy(rest, -1);
        hygiene = changeBy(hygiene, -1);
        entertainment = changeBy(entertainment, -2);
        food = changeBy(food, -3);
    }
  }

  function isSleeping() { return status === "SLEEPING"; }
  function isDead() { return status === "DEAD"; }

  function wake() {
    if(isDead()) { return; }
    if(isSleeping()) { status = "NEUTRAL"; }
    updateUI();
  }

  function sleep() {
    if(isDead()) { return; }
    status = "SLEEPING";
    updateUI();
  }

  function eat() {
    if(isDead() || isSleeping()) { return; }
    food = changeBy(food, 5);
    hygiene = changeBy(hygiene, -2);
    entertainment = changeBy(entertainment, 2);
    updateUI();
  }

  function entertain() {
    if(isDead() || isSleeping()) { return; }
    entertainment = changeBy(entertainment, 5);
    rest = changeBy(rest, -4);
    hygiene = changeBy(hygiene, -1);
    updateUI();
  }

  function wash() {
    if(isDead() || isSleeping()) { return; }
    hygiene = changeBy(hygiene, 5);
    entertainment = changeBy(entertainment, 2);
    updateUI();
  }

  function tick() {
    ++ticks;
    advanceState();
    updateMood();
    updateUI();
    setTimeout(tick, 2000);
  }

  $('#wake').on('click', wake);
  $('#sleep').on('click', sleep);
  $('#eat').on('click', eat);
  $('#wash').on('click', wash);
  $('#entertain').on('click', entertain);
  tick();
});
