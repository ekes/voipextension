<?php

function voipextension_basic_script() {
  $script = new VoipScript('voipscript_basic');

  $script->addLabel('request_extension');
  $script->addGetInput(t('Please enter the number of the extension you would like followed by a #. Or press * to hang up.'));
  $script->addGotoIf('no_input_received', "^%input_digits == ''");
  $script->addGotoIf('hang_up', "^%input_digits == '*'");
  $script->addGosub('voipextension_gosub', array('number' => "^%input_digits"));
  $script->addLabel('no_input_received');
  $script->addSay('No input received. Please try again.');
  $script->addGoto('request_extension');
  return $script;
}

function voipextension_gosub_script($eid) {
  // remove the terminating # if it is there.
  if (substr($eid, -1) == '#') {
    $eid = substr($eid, 0, -1);
  }

  // @todo load api, when in a seperate file.

  $script = new VoipScript('voipextension_gosub');
  if ($extension = voipextension_load($eid) && $gosub = voipextension_gosub($extension)) {
    $title_prompt = voipextension_say_title($extension);
    $script->addSay('Transfering to ');
    $scirpt->addSay($title_prompt);
    $script->addGosub($gosub['script_name'], $gosub['script_arguments']);
  }
  else {
    $script->addSay('Unknown extension number ' . $eid);
  }
  $script->addReturn();
 
  return $script;
}
