<?php
/**
 * Created by JetBrains PhpStorm.
 * User: creynder
 * Date: 20/04/12
 * Time: 14:24
 * To change this template use File | Settings | File Templates.
 */


function scoga_get_player_uids_by_team_nid( $team_nid ){
    $query = new EntityFieldQuery();
    $entity_type = 'user';
    $result = $query
        ->entityCondition( 'entity_type', $entity_type )
        ->fieldCondition( 'field_user_team', 'target_id', $team_nid, '=' )
        ->execute();

    $uids = array_keys($result[ $entity_type ]);
    return $uids;
}

function scoga_get_player_vos_by_UUIDs( $player_UUIDs ){
    return scoga_get_output( $player_UUIDs, 'scoga_get_player_vo_by_UUID' );
}

/**
 * @param $player_UUID UUID for the player entity
 * @return array
 */
function scoga_get_player_vo_by_UUID( $player_UUID ){
    $player_uid = scoga_get_eid_by_uuid( $player_UUID, 'user' );
    if( $player_uid ){
        return scoga_get_player_vo_by_uid( $player_uid );
    }
}

/**
 * retrieve a list of player VO's
 * @param array $player_uids
 * @return array|bool of player VO's
 */
function scoga_get_player_vos_by_uids( $player_uids ){
    return scoga_get_output( $player_uids, 'scoga_get_player_vo_by_uid' );
}

/**
 * get player VO
 * @param $player_uid
 * @return array player VO
 */
function scoga_get_player_vo_by_uid( $player_uid ){
    $player_profile = user_load( $player_uid );
    if( $player_profile ){
        $result = scoga_create_player_vo( $player_profile );
        $result = _inject_player_points( $result );
        return $result;
    }
}

/**
 * retrieve user id's by card ID
 * @param $card_id unique card ID, has nothing to do with Drupal ID's [!]
 * @return array of uid's
 */
function scoga_get_player_uids_by_card_id( $card_id ){
    $user_entity_type = 'user';
    $user_query = new EntityFieldQuery();

    //get user profile by card ID if possible
    $user_entities = $user_query
        ->entityCondition( 'entity_type', $user_entity_type )
        ->fieldCondition( 'field_card_id', 'value', $card_id, '=', 0 )
        ->execute();

    if( array_key_exists( $user_entity_type, $user_entities ) ){
        return array_keys( $user_entities[ $user_entity_type ] );
    }

    return array();
}

/**
 * @param $card_id unique card ID, has nothing to do with Drupal ID's [!]
 * @return bool|mixed
 */
function scoga_get_player_entity_by_card_id( $card_id ){
    $user_uids = scoga_get_player_uids_by_card_id( $card_id );
    switch( count( $user_uids ) ){
        case 1 : {
            //exactly one match, as it should
            return user_load( $user_uids[ 0 ] );
        }
        case 0 : {
            return FALSE;
        }
        default : {
            // multiple matches
            //TODO determine what to do with multiple matches
            scoga_throw_fatal_error( 'card registered multiple times' );
        }
    }
}

/**
 * create player VO
 * @param $player_entity
 * @return array
 */
function scoga_create_player_vo( $player_entity ){
    $lang = LANGUAGE_NONE;
    $output = array(
        'uid'           => $player_entity->uid,
        'username'      => $player_entity->name,
        'uuid'          => $player_entity->uuid,
        'registered'    => $player_entity->field_user_registered[ $lang ][ 0 ][ 'value' ],
        'team_nid'      => $player_entity->field_user_team[ $lang ][ 0 ][ 'target_id' ],
        'firstname'     => $player_entity->field_user_firstname[ $lang ][ 0 ][ 'value' ],
        'surname'       => $player_entity->field_user_surname[ $lang ][ 0 ][ 'value' ],
        'avatar_url'    => image_style_url( 'avatar_pinhead', $player_entity->picture->uri ),
        'points'        => 0,
    );
    return $output;
}


/**
 * create a player entity
 * @param $card_id unique card ID, has nothing to do with Drupal ID's [!]
 * @return bool|stdClass
 */
function scoga_create_player_entity( $card_id, $team_nid ){
    $lang = LANGUAGE_NONE;
    $user_profile = new stdClass();
    $user_profile->name = 'kliek user '. $card_id;
    $user_profile->status = 1; //0->blocked, 1->active
    $user_profile->mail = 'none';
    $user_profile->field_team_reference[ $lang ][ 0 ] = array(
        'nid' => $team_nid
    );
    $user_profile->roles = array(
        2 => 'authenticated user',
        4 => 'kliek user'
    );
    $user_profile->field_card_id[ $lang ][ 0 ] = array(
        'value' => $card_id
    );
    $user_profile->is_new = TRUE;

    $user_account = user_save( $user_profile );
    dd( $user_account );
    return $user_account;
}

/**
 * @private
 * @param $player_object
 * @return array
 */
function _inject_player_points( $player_object ){

    //TODO: use userpoints_service method and filter out the relevant point categories,
    //now it returns ALL the user points
    //use: userpoints_get_current_points
    $query =
        "SELECT points "
        . "FROM {userpoints_total} "
        . "WHERE uid = :target_uid"
    ;
    $rows = db_query( $query, array(':target_uid' => $player_object[ 'uid' ] ) );

    foreach( $rows as $row ){
        $player_object[ 'points' ] += $row->points;
    }

    return $player_object;
}