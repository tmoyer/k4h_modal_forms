<?php

/* TODO: add admin area where we can set variables to
   hold which nodes to add this to, which fields to redirect to,
   and whether to show on submit or whenever the modal is closed.
*/
/**
 * @file
 * Administration function for k4h_modal_forms module.
 */

/**
 * Default settings form to list publishers that have given their permission to bundle in offline toolkits.
 */
function k4h_modal_forms_settings() {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('K4H Modal Forms settings'),
    '#collapsible' => FALSE,
  );

  $form['settings']['k4h_modal_forms_nodes'] = array(
    '#type' => 'textarea',
    '#title' => t('Nodes and settings for modal forms in content.'),
    '#default_value' => variable_get('k4h_modal_forms_nodes', ''),
    '#size' => 6,
    '#description' => t('Enter the node id (nid), the webform\'s nid, machine name of which field to redirect to (i.e. field_resource_file), and either "submit" or "both" for whether to redirect only when the form is submitted or just whenever the form it submitted or closed, and optionally text to be used for the cancel link (the default is "No thanks"). The entry must be in this format: "nid:nid:field:submit:cancel text" (i.e. "234:54:field_resource_file:both:No thanks--proceed to publication").'),
    '#wysiwyg' => false,
  );
/*
  $form['settings']['toolkit_teaser_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Toolkit teaser length.'),
    '#default_value' => variable_get('toolkits_teaser_length', '600'),
    '#size' => 6,
    '#description' => t('The maximum length (in characters) of other teasers in toolkits (i.e. toolkit listings and taxonomy pages).'),
  );

  $form['settings']['k4h_modal_forms_publisher_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label to display above list of publishers (see list below).'),
    '#default_value' => variable_get('k4h_modal_forms_publisher_label', 'Check if the publisher is among those that have already given consent to bundle resources:'),
    '#size' => 60,
    '#description' => t('This label will display directly above the list of publishers in the toolkit resource node form.'),
  );

  $publishers = _k4h_modal_forms_default_publishers();

  $form['settings']['k4h_modal_forms_publishers'] = array(
    '#type' => 'textarea',
    '#title' => t('List publishers that have given permission to bundle their resources in offline Toolkits.'),
    '#default_value' => variable_get('k4h_modal_forms_publishers', implode("\n", $publishers)),
    '#rows' => 10,
    '#resizable' => TRUE,
    '#maxlength' => 800000,
    '#description' => t('List all publishers that have given their permission to include their resources in our offline Toolkit bundles.'),
  );
*/
  $form['#validate'][] = 'k4h_modal_forms_settings_form_validate';

  return system_settings_form($form);
}

function k4h_modal_forms_settings_form_validate($form, &$form_state) {
  // Loop thru each line of entry
  $lines = preg_split('/\r\n|\r|\n/', $form_state['values']['k4h_modal_forms_nodes']);
  foreach ($lines as $num => $line) {
    $inputs = explode(':', $line);
    if (count($inputs) < 4) {
      form_set_error('k4h_modal_forms_nodes', t('You must enter 4 values per line (with an optional 5th value), delimited by a ":" (colon) in this format: "nid:nid:field:[submit or both]:cancel link text" (on line !num)', array('!num' => $num + 1)));
    }
    if (!is_numeric($inputs[0])) {
      form_set_error('k4h_modal_forms_nodes', t('The 1st part of each entry must be the numeric id for a node. Please check and retry (on line !num).', array('!num' => $num + 1)));
    }
    if (!is_numeric($inputs[1])) {
      form_set_error('k4h_modal_forms_nodes', t('The 2nd part of each entry must be the numeric id for a node. Please check and retry (on line !num).', array('!num' => $num + 1)));
    }
    if (substr($inputs[2], 0, 6) != 'field_') {
      form_set_error('k4h_modal_forms_nodes', t('The 3rd part of each entry must be a field and start with "field_". Please check and retry (on line !num).', array('!num' => $num + 1)));
    }
    if ($inputs[3] != 'submit' && $inputs[3] != 'both') {
      form_set_error('k4h_modal_forms_nodes', t('The 4th part of each entry must be either "submit" or "both". Please check and retry (on line !num).', array('!num' => $num + 1)));
    }
  }
}

function k4h_modal_forms_settings_form_submit($form, &$form_state) {
  // Save form settings.
  system_settings_form_submit($form, $form_state);
}
