<?php

function voipextension_basic_script() {
  $script = new VoipScript('voipscript_basic');

  $script->addLabel('request_extension');
  // @todo variable length addGetInput is broken with tropo at least
  //       have NULL as length it returns after the first key
  //       have a larger number and '#' as the end_key and there is no
  //       value placed in %input_digits
  // fixed at 3 digits for now 
  $script->addGetInput(t('Please enter the number of the extension you would like followed by a #. Or press * to hang up.'), 3, '#', 5);
$script->addLog('%input_digits', 'getInput result');
  $script->addGotoIf('no_input_received', "^%input_digits == ''");
  $script->addGotoIf('hang_up', "^%input_digits == '*'");
  $script->addGosub('voipextension_gosub', array('number' => "%input_digits")); 
  $script->addGoto('request_extension');
  $script->addLabel('no_input_received');
  $script->addSay('No input received. Please try again.');
  $script->addGoto('request_extension');
  $script->addLabel('hang_up');
  $script->addSay('Goodbye');
  $script->addReturn();
  return $script;
}

function voipextension_gosub_script($eid) {
  // remove the terminating # if it is there.
  if (substr($eid, -1) == '#') {
    $eid = substr($eid, 0, -1);
  }

  // @todo load api, when in a seperate file.

  $script = new VoipScript('voipextension_gosub');
  if ($extension = voipextension_load($eid)) {
    $gosub = voipextension_gosub($extension);
    $title_prompt = voipextension_say_title($extension);
    $script->addSay('Transfering to extension number ' . $eid . ' .');
    $script->addSay($title_prompt);
    $script->addGosub($gosub['script_name'], $gosub['script_arguments']);
  }
  else {
    $script->addSay('Unknown extension number ' . $eid);
  }
  $script->addReturn();
 
  return $script;
}
