<?php
/**
 * Created by JetBrains PhpStorm.
 * User: creynder
 * Date: 20/04/12
 * Time: 14:41
 * To change this template use File | Settings | File Templates.
 *
 * Invoked from services
 */

function scoga_core_campaign_resource_index( $params ){
    module_load_include( 'api.campaign.inc', 'scoga_core' );
    if( array_key_exists( 'uuid', $params ) ){
        return scoga_get_campaign_vos_by_UUIDs( $params[ 'uuid' ] );
    }

    if( array_key_exists( 'nid', $params ) ){
        return scoga_get_campaign_vos_by_nids( $params[ 'nid' ] );;
    }

    return FALSE;
}

function scoga_core_team_resource_index( $params ){
    module_load_include( 'api.team.inc', 'scoga_core' );
    if( array_key_exists( 'uuid', $params ) ){
        return scoga_get_team_vos_by_UUIDs( $params[ 'uuid' ] );
    }

    if( array_key_exists( 'nid', $params ) ){
        return scoga_get_team_vos_by_nids( $params[ 'nid' ] );
    }
}

function scoga_core_player_resource_index( $params ){
    module_load_include( 'api.player.inc', 'scoga_core' );
    if( array_key_exists( 'uuid', $params ) ){
        return scoga_get_player_vos_by_UUIDs( $params[ 'uuid' ] );
    }

    if( array_key_exists( 'uid', $params ) ){
        return scoga_get_player_vos_by_uids( $params[ 'uid' ] );
    }
}

function scoga_core_transaction_resource_index( $params ){
    module_load_include( 'api.transaction.inc', 'scoga_core' );
    return scoga_get_transaction_vos_by_period( $params[ 'begin_time' ], $params[ 'end_time' ] );
}