<?php

function _voipextension_get_basic_menu_script() {
  $script = new VoipScript('voipextension_basic_menu');

  $prompt = variable_get('voipextension_prompt_msg', t('Please enter the desired extension number followed by the pound key. Or press star to go back.'));

  $script->addLabel('request_extension');
  $script->addGetInput($prompt, 5, '#', 3);
  $script->addGotoIf('no_input_received', "^%input_digits == ''");
  $script->addGotoIf('go_back', "^%input_digits == '*'");
  $script->addGosub('voipextension_play_extension', array('eid' => "%input_digits")); 
  $script->addGoto('request_extension');

  $script->addLabel('no_input_received');
  $script->addSay(variable_get('voipextension_noinput_msg', t('No input received. Please try again.')));
  $script->addGoto('request_extension');

  $script->addLabel('go_back');
  $script->addReturn();

  return $script;
}

function _voipextension_get_play_extension_script($eid, $options=array()) {
  // @todo load api, when in a separate file.

  $script = new VoipScript('voipextension_play_extension');
  // using voipscript %number replacement
  $script->setVar('number', $eid);

  if (($extension = voipextension_load($eid)) && $extension['enabled']) {
    $gosub = voipextension_get_script($extension);
    if (! $transferring = variable_get('voipextension_transferring_msg', FALSE)) {
      $transferring = t('Transferring to extension %number. ');
    }
    $script->addSay($transferring);
    $arguments = array_merge($gosub['script_arguments'], $options);
    // include the extension in the arguments
    $arguments['eid'] = $eid;
    $script->addGosub($gosub['script_name'], $arguments);
  }
  else {
    if (! $unknown = variable_get('voipextension_unknown_msg', FALSE)) {
      $unknown = t('Unknown extension %number. ');
    }
    $script->addSay($unknown);
  }
  $script->addReturn();
 
  return $script;
}

/**
 * Basic script that reads a voipextension directory and executes the script
 * associated with the selected extension number. This function might be called
 * directly by other modules to create their own directories.
 *
 * @param @module
 * String module name
 *
 * @param @extension_type
 * String with the type of the extensions to be included in the directory
 *
 * @param @options
 * Array with optinal parameters to change the prompts of the script
 *
 * @return
 * Sets script variable 'selected_extension' with the extension number selected,
 * or NULL in case of timeout or invalid input. 
 */
function voipextension_get_directory_script($module, $extension_type='', $options=array()) {

  $script = new VoipScript('voipextension_directory');

  $rx = voipextension_load_module_extensions($module, $extension_type);

  if(!$rx) {
    $p_no_extensions = t('This directory is currently empty. ');
    if($options['p_no_extensions']) {
      $p_no_extensions = $options['p_no_extensions'];
    }
    $script->addSay($p_no_extensions);
  }
  else {
    $p_directory_name = t("Extension's directory. ");
    if($options['p_directory_name']) {
      $p_directory_name = $options['p_directory_name'] . ' ';
    }
    $p_instructions = t("At any time, press the desired extension number or press the pound key to go back to the previous menu. ");
    if($options['p_directory_instructions']) {
      $p_instructions = $options['p_directory_instructions'] . ' ';
    }
    $p_no_input = t('No input received. Please try again. ');
    if($options['p_no_input']) {
      $p_no_input = $options['p_no_input'] . ' ';
    }
    $p_invalid_extension = t('Invalid extension number. Please try again. ');
    if($options['p_invalid_extension']) {
      $p_invalid_extension = $options['p_invalid_extension'] . ' ';
    }

    // by default, run the script associated with the selected extension
    $run_scripts = TRUE;
    if($options['run_scripts']) {
      $run_scripts = $options['run_scripts'] . ' ';
    }

    $prompt = $p_directory_name . $p_instructions;

    // sort extensions by their text title
    function _voipextension_sort_by_title($x1, $x2) {
        return strnatcmp($x1['title'], $x2['title']); }
    uasort($rx, '_voipextension_sort_by_title');

    foreach($rx as $x) {
      $number = $x['eid'];
      $audio_title = voipextension_get_title($x);
      $x_numbers[] = $number;
      $prompt .= t('for @title, press @number. ',
                   array('@title' => $audio_title, '@number' => $number));
    }

    $script->addSet('__dir_counter', 0);
    $script->addSet('selected_extension', NULL);
    $s_x_numbers = serialize($x_numbers);

    $script->addLabel('read_directory');
    $script->addGetInput($prompt, NULL, '', 3);
    $script->addGotoIf('no_input', '^is_null(%input_digits)');
    $script->addGotoIf('go_back', '^%input_digits == "#"');
    $script->addGotoIf('invalid_extension',
                      "^!in_array(%input_digits, unserialize('$s_x_numbers'))");

    // handle valid extension number
    $script->addSet('selected_extension', '%input_digits');
    if($run_scripts) {
      $options['eid'] = '%selected_extension';
      $script->addGosub('voipextension_play_extension', $options);
    }

    $script->addGoto('go_back');

    $script->addLabel('no_input');
    $script->addSet('__dir_counter', '^%__dir_counter + 1');
    $script->addGotoIf('go_back', "^%__dir_counter >= 3");
    $script->addSay($p_no_input);
    $script->addGoto('read_directory');

    $script->addLabel('invalid_extension');
    $script->addSet('__dir_counter', '^%__dir_counter + 1');
    $script->addGotoIf('go_back', "^%__dir_counter >= 3");
    $script->addSay($p_invalid_extension);
    $script->addGoto('read_directory');

    $script->addLabel('go_back');
    $script->addUnset('__dir_counter');
  }

  return $script;
}
   
