<?php
/**
 * User: creynder
 * Date: 20/04/12
 * Time: 14:24
 */

class Campaign {
    public $nid = 0;
    public $title;
    public $end_date;
    public $begin_date;
}

//    function scoga_get_campaign_vos_by_UUIDs( $campaign_UUIDs ){
//        return scoga_get_output( $campaign_UUIDs, 'scoga_get_campaign_vo_by_UUID' );
//    }
//
//    function scoga_get_campaign_vo_by_UUID( $campaign_UUID ){
//        $campaign_nid = scoga_get_eid_by_uuid( $campaign_UUID );
//        if( $campaign_nid ){
//            return scoga_get_campaign_vo_by_nid( $campaign_nid );
//        }
//
//        return FALSE;
//    }

/**
 * @return array
 */
function scoga_get_campaign_vos(){
    $nids = scoga_get_campaign_nids();
    $vos = scoga_get_campaign_vos_by_nids( $nids );
    return $vos;
}

function scoga_get_campaign_nids(){
    $entity_type = 'node';
    $query = new EntityFieldQuery();
    $result = $query
        ->entityCondition( 'entity_type', $entity_type )
        ->entityCondition( 'bundle', SCOGA_CAMPAIGN_NODETYPE )
        ->execute()
    ;
    if( isset( $result[ $entity_type ] )){
        return array_keys( $result[ $entity_type ] );
    }

    return array();
}

/**
 * @param string|array $campaign_nids
 *
 * @return array
 */
function scoga_get_campaign_vos_by_nids( $campaign_nids ){
    return scoga_get_output( $campaign_nids, 'scoga_get_campaign_vo_by_nid' );
}

/**
 * @param $campaign_nid
 *
 * @return Campaign
 */
function scoga_get_campaign_vo_by_nid( $campaign_nid ){
    $campaign_node = node_load( $campaign_nid );
    if( $campaign_node ){
        return scoga_create_campaign_vo( $campaign_node );
    }

    return FALSE;
}

/**
 * @param $campaign_node
 *
 * @return Campaign
 */
function scoga_create_campaign_vo( $campaign_node ){
    //dd( $campaign_node, 'campaign node');
    $begin_time_tstamp = scoga_get_field_value( $campaign_node, 'field_campaign_period' );
    $end_time_tstamp = scoga_get_field_value( $campaign_node, 'field_campaign_period', array( 'field_column' => 'value2' ) );

    $output = new Campaign();
    $output->begin_date = $begin_time_tstamp;
    $output->end_date = $end_time_tstamp;
    $output->nid = $campaign_node->nid;
    $output->title = $campaign_node->title;
    return $output;
}


/**
 *
 */
function scoga_determine_current_campaign(){
    $old_nid = scoga_get_current_campaign_nid();
    $new_nid = scoga_get_campaign_nid_for_time();

    if( $new_nid != $old_nid ){
        scoga_finish_campaign( $old_nid );
        scoga_start_campaign( $new_nid );
    }
}

    function scoga_finish_campaign( $old_nid ){
        if( $old_nid > SCOGA_NULL_CAMPAIGN_NID ){
            module_invoke_all( 'scoga_current_campaign_finished', $old_nid );
        }
    }

    function scoga_start_campaign( $campaign_nid ){
        scoga_set_current_campaign_nid( $campaign_nid );
        if( $campaign_nid > SCOGA_NULL_CAMPAIGN_NID ){
            module_invoke_all( 'scoga_current_campaign_started', $campaign_nid );
        }
    }

/**
 * @param null $timestamp
 *
 * @return int returns nid of campaign with a date range that encompasses the <code>timestamp</code>. Returns -1,
 * if no such campaign was found
 */
function scoga_get_campaign_nid_for_time( $timestamp = NULL ){
    $query = new EntityFieldQuery();
    $entity_type = 'node';
    isset( $timestamp ) || $timestamp = time();

    $result = $query
        ->entityCondition( 'entity_type', $entity_type )
        ->entityCondition( 'bundle', SCOGA_CAMPAIGN_NODETYPE )
        ->propertyCondition( 'status', '1' )
        ->fieldCondition( 'field_campaign_period', 'value', $timestamp, '<=' )
        ->fieldCondition( 'field_campaign_period', 'value2', $timestamp, '>' )
        ->execute();

    if( $result ){
        //campaign found that should be activated if it isn't already
        $partial_node = reset( $result[ $entity_type ] );
        $new_nid = $partial_node->nid;
    } else{
        $new_nid = SCOGA_NULL_CAMPAIGN_NID;
    }
    return $new_nid;
}

function scoga_get_current_campaign_vo(){
    $nid = scoga_get_current_campaign_nid();
    if( $node = scoga_get_campaign_vo_by_nid( $nid ) ){
        return $node;
    }

    return new Campaign();
}

/**
 *@return array containing days, hours, minutes, seconds to end of active campaign
 */
function scoga_get_current_campaign_expiration_time()
{
    define('DAY',60*60*24,TRUE);
    define('HOUR',60*60,TRUE);
    define('MINUTE',60,TRUE);
    $time_interval_elements = array();

    $campaign_node = node_load(scoga_get_campaign_nid_for_time(time()));
    $campaign_end_timestamp = $campaign_node->field_campaign_period['und'][0]['value2'];
    $time_interval = $campaign_end_timestamp - time();

    $days = floor($time_interval/DAY);
    $time_interval_elements[] = $days;
    $hours = floor(($time_interval-$days*DAY)/HOUR);
    $time_interval_elements[] = $hours;
    $minutes = floor(($time_interval-$days*DAY-$hours*HOUR)/MINUTE);
    $time_interval_elements[] = $minutes;
    $seconds = floor($time_interval-$days*DAY-$hours*HOUR-$minutes*MINUTE);
    $time_interval_elements[] = $seconds;

    return $time_interval_elements;
}
