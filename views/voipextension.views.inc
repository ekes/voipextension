<?php

/**
 * @file
 *  Provide views data and handlers for voipextension.module.
 */

/**
 * Implementation of hook_views_data().
 */
function voipextension_views_data() {
  $data = array();

  // table information
  $data['voipextension']['table']['group'] = t('VoIP extension');

  // available as a base table
  $data['voipextension']['table']['base'] = array(
    'field' => 'eid',
    'title' => t('VoIP extension'),
    'help' => t('Extension numbers.'),
  );

  // other tables should define how they relate to module, module_id 

  // fields
  $data['voipextension']['eid'] = array(
    'title' => t('Extension number'),
    'help' => t('The number of the extension'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'title',
      'zero is null' => TRUE,
    ),
    'filter' => array(
       //
    ),
  );

  $data['voipextension']['title'] = array(
    'title' => t('Title'),
    'help' => t('Text title of the extension.'),
    'field' => array(
      'handler' => 'views_handler_field_xss', // @todo we're just looking at check_plain?
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['voipextension']['module'] = array(
    'title' => t('Module'),
    'help' => t("The name of the module that created the extension."),
    'field' => array(
      'handler' => 'views_handler_field_xss',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['voipextension']['module_id'] = array(
    'title' => t('Module Id'),
    'help' => t("The ID given by the extension's module."),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['voipextension']['extension_type'] = array(
    'title' => t('Extension type'),
    'help' => t("The type of extension as defined by the extension's module."),
    'field' => array(
      'handler' => 'views_handler_field_xss',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['voipextension']['description'] = array(
    'title' => t('Description'),
    'help' => t('Text description of the extension, if set.'),
    'field' => array(
      'handler' => 'views_handler_field_xss', // @todo need check_plain
    ),
  );

  $data['voipextension']['script'] = array(
    'title' => t('Script'),
    'help' => t('The name of the VoIP script, if set as fixed.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['voipextension']['directory_type'] = array(
    'title' => t('Directory listing'),
    'help' => t('If the extension should be listed in a directory.'),
    'field' => array(
      // for now we're just bool
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Listed in directory'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['voipextension']['enabled'] = array(
    'title' => t('Enabled'),
    'help' => t('If the extension is enabled.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Enabled'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
