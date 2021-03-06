<?php
/**
 * @file
 * scoga_feature.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function scoga_feature_default_services_endpoint() {
  $export = array();

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'rest';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'rest';
  $endpoint->authentication = array(
    'services' => 'services',
  );
  $endpoint->server_settings = array(
    'rest_server' => array(
      'formatters' => array(
        'json' => TRUE,
        'bencode' => FALSE,
        'jsonp' => FALSE,
        'php' => FALSE,
        'rss' => FALSE,
        'xml' => FALSE,
        'yaml' => FALSE,
      ),
      'parsers' => array(
        'application/json' => TRUE,
        'application/x-www-form-urlencoded' => TRUE,
        'multipart/form-data' => TRUE,
        'application/vnd.php.serialized' => FALSE,
        'application/x-yaml' => FALSE,
      ),
    ),
  );
  $endpoint->resources = array(
    'scoga_teams' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'scoga_campaigns' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'scoga_sync' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'scoga_network' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'scoga_checkin' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'scoga_reader' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'scoga_pages' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'scoga_config' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'scoga_players' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'scoga_events' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'node' => array(
      'alias' => 'node',
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
      'relationships' => array(
        'files' => array(
          'enabled' => 1,
        ),
        'comments' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'system' => array(
      'alias' => 'system',
      'actions' => array(
        'connect' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'user' => array(
      'alias' => 'user',
      'actions' => array(
        'login' => array(
          'enabled' => 1,
        ),
        'logout' => array(
          'enabled' => 1,
        ),
      ),
    ),
  );
  $endpoint->debug = 0;
  $export['rest'] = $endpoint;

  return $export;
}
