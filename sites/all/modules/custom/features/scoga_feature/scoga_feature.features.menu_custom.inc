<?php
/**
 * @file
 * scoga_feature.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function scoga_feature_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: main-menu.
  $menus['main-menu'] = array(
    'menu_name' => 'main-menu',
    'title' => 'Main menu',
    'description' => 'The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.',
  );
  // Exported menu: menu-footer.
  $menus['menu-footer'] = array(
    'menu_name' => 'menu-footer',
    'title' => 'Footer',
    'description' => '',
  );
  // Exported menu: menu-score-navigation-bar.
  $menus['menu-score-navigation-bar'] = array(
    'menu_name' => 'menu-score-navigation-bar',
    'title' => 'Score navigation bar',
    'description' => '',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Footer');
  t('Main menu');
  t('Score navigation bar');
  t('The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.');


  return $menus;
}
