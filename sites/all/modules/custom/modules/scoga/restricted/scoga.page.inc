<?php

    class assignment_vo
    {
        public $content_type;
        public $label;
        public $header;
        public $campaign_ref_field;
        public $team_ref_field;
        public $user_team;
        public $current_campaign_id;
        public $user;
    }

    /*
     * Construct the list of /not_checkin candidates that the logged in user has not checkin in with in the current campaign
     * and from the same team as the player
     * @return string
     */
    function scoga_get_list_of_not_checkin_parnters()
    {
        module_load_include( 'api.inc', 'scoga', 'api/scoga' );
        $active_relationsip_tid = scoga_get_current_combocheckin_rtid();
        global $user;
        $user_data = user_load($user->uid);
        $user_team = $user_data->field_user_team['und']['0']['target_id'];
        global $user;
        $no_checkin_candidates = array();
        $query = "SELECT DISTINCT
                  users.name AS users_name,
                  users.uid AS uid
                  FROM users users
                  LEFT JOIN field_data_field_user_active = field_data_field_user_active
                  ON users.uid = field_data_field_user_active.entity_id
                  LEFT JOIN field_data_field_user_team field_data_field_user_team
                  ON users.uid = field_data_field_user_team.entity_id
                  WHERE field_data_field_user_active.field_user_active_value=1
                  AND field_data_field_user_team.field_user_team_target_id = :team_id
                  AND uid !=:uid
                  AND uid NOT IN
                  (
                      SELECT DISTINCT
                      users.uid AS uid
                      FROM users users
                      LEFT JOIN user_relationships user_relationships
                      ON users.uid = user_relationships.requestee_id
                      WHERE user_relationships.requester_id = :uid
                      AND user_relationships.rtid = :active_rel_id
	              )";
        $result = db_query($query, array(':team_id'=>$user_team,
                                         ':uid'=>$user->uid,
                                         ':active_rel_id' => $active_relationsip_tid
                                        ));
        foreach ($result as $row)
        {
            if (strpos($row->users_name,'Vogel')===false)
            {
                $no_checkin_candidates[$row->uid] = $row->users_name;
            }
        }
        //theme the output
        $output = '<div class="help"><a >?</a></div>';
        $output .= theme('no_checkin',array('names' => array_values($no_checkin_candidates),'uids' => array_keys($no_checkin_candidates)));
        return $output;

    }

    /*
     * Construct the list of /get_points
     * @return string
     */
    function scoga_overview_of_assignments_to_get_point()
    {
        module_load_include( 'api.campaign.inc', 'scoga', 'api/scoga' );
        $current_campaign_id = scoga_get_current_campaign_nid();
        $output='';
        if ($current_campaign_id>0)
        {
            global $user;
            $user_data = user_load($user->uid);
            $user_team = $user_data->field_user_team['und']['0']['target_id'];
            $output='';
            $output.= '<div class="help"><a >?</a></div>';
            $assignment_titles = array();
            $assignment_paths = array();
            //add time interval to end of current campaign
            $time_interval_elements = scoga_get_current_campaign_expiration_time();
            $output .= theme('time_interval',array('interval_elements' => $time_interval_elements));
            //add link to node based on path stored in SCOGA_SETTING_CHECKIN_EXPLANATION_NID to output
            $assignment_titles[] = t('Check-in : find new partners');
            $assignment_paths[] = SCOGA_NOT_PARTNER_PAGE;
            //add link to wish creation page to output
            $assignment_titles[] = t ('Make a wish');
            $assignment_paths[] = 'node/add/nodetype-wished-item';
            //add other playable elements to output
            $assignment_vo = new assignment_vo();
            module_load_include( 'api.team.inc', 'scoga', 'api/scoga' );
            $opposing_teams = scoga_get_opposing_teams_nid($user_team,$current_campaign_id);
            $assignment_vo -> opposing_team = $opposing_teams[0];
            //add events
            $assignment_vo -> content_type = 'nodetype_event';
            $assignment_vo -> label = 'Event';
            $assignment_vo -> header = 'Active events';
            $assignment_vo -> campaign_ref_field = 'field_campaign';
            $assignment_vo -> team_ref_field = 'field_event_team';
            $assignment_vo -> user_team = $user_team;
            $assignment_vo -> current_campaign_id = $current_campaign_id;
            _scoga_add_trans_team_assignment_to_output($assignment_vo,$assignment_titles,$assignment_paths);
            //add polls
            $assignment_vo -> content_type = 'Poll';
            $assignment_vo -> label = 'Question';
            $assignment_vo -> header = 'Active questions';
            $assignment_vo -> team_ref_field = 'field_poll_team';
            _scoga_add_trans_team_assignment_to_output($assignment_vo,$assignment_titles,$assignment_paths);
            //add questions
            $assignment_vo -> content_type = 'question';
            $assignment_vo -> label = 'Writein';
            $assignment_vo -> header = 'Active writein questions';
            $assignment_vo -> team_ref_field = NULL;
            $assignment_vo -> campaign_ref_field = 'field_campaign';
            _scoga_add_assignments_to_output($assignment_vo,$assignment_titles,$assignment_paths);
            //add generic assignments
            $assignment_vo -> content_type = 'generic_assignment';
            $assignment_vo -> label = 'Generic assignment';
            $assignment_vo -> header = 'Active generic assignments';
            $assignment_vo -> team_ref_field = 'field_genric_assignment_team';
            _scoga_add_trans_team_assignment_to_output($assignment_vo,$assignment_titles,$assignment_paths);
            //add geo challenges
            $assignment_vo -> content_type = 'geo_challenge';
            $assignment_vo -> label = 'GeoChallenge';
            $assignment_vo -> header = 'Active GeoChallenges';
            //GeoChallenges are created for all teams
            $assignment_vo -> team_ref_field = NULL;
            //$assignment_vo -> campaign_ref_field = 'field_geochallenge_ref_campaign';
            $assignment_vo -> campaign_ref_field = 'field_campaign';
            _scoga_add_assignments_to_output($assignment_vo,$assignment_titles,$assignment_paths);
            //add POI confirmations
            _scoga_add_POI_confirmations_to_output($assignment_titles,$assignment_paths);
            //add wishes
            $assignment_vo -> content_type = 'nodetype_wished_item';
            $assignment_vo -> label = 'Wish';
            $assignment_vo -> header = 'Posted wishes';
            $assignment_vo -> campaign_ref_field = 'field_wisheditem_campaign';
            //wishes to be answered should have been posted by someone in opposing team
            $assignment_vo -> user_team = $assignment_vo -> opposing_team;
            _scoga_add_wishes_to_output($assignment_vo,$assignment_titles,$assignment_paths);
            //add wishes posted by user
            $assignment_vo -> content_type = 'nodetype_wished_item';
            $assignment_vo -> label = 'Wish by you';
            $assignment_vo -> header = 'Wishes by you';
            $assignment_vo -> campaign_ref_field = 'field_wisheditem_campaign';
            $assignment_vo -> user = $user->uid;
            _scoga_add_own_wishes_to_output($assignment_vo,$assignment_titles,$assignment_paths);
            $output .= theme('get_points',array('assignment_titles' => $assignment_titles,'assignment_paths' => $assignment_paths));
        }
        else
        {
            $output .= '<div class="no_active"">'.t('No campaign is currently active').'</div>';
        }
        return $output;
    }

    /**
     * Loads nodes from DB and adds them to the output, preceded by a header
     */
    function _scoga_add_assignments_to_output($assignment_vo,&$titles,&$paths)
    {
        $nodes_properties = _scoga_get_assignments_by_type($assignment_vo,$titles,$paths);
        if (isset($nodes_properties['node']))
        {
            $nodes = node_load_multiple(array_keys($nodes_properties['node']));
            _scoga_process_assignment_query_results($assignment_vo,$nodes,$titles,$paths);
        }
    }

    /**
     * @param $assignment_vo
     * @param $assignment_titles
     * @param $assignment_paths
     */
    function _scoga_add_wishes_to_output($assignment_vo,&$assignment_titles,&$assignment_paths)
    {
        $nodes = array();
        /*$query = "SELECT nid FROM node
        LEFT JOIN field_data_field_wisheditem_campaign AS campaign ON node.nid=campaign.entity_id
        LEFT JOIN field_data_field_wisheditem_state AS state on node.nid=state.entity_id
        LEFT JOIN field_data_field_user_team AS team ON node.uid=team.entity_id
        WHERE node.type = 'nodetype_wished_item'
        AND field_wisheditem_state_value = 'open'
        AND team.field_user_team_target_id = :user_id
        AND campaign.field_wisheditem_campaign_target_id= :campaign_id";
        $result = db_query($query,
            array(':user_id'=>$assignment_vo -> user_team,
                  ':campaign_id' =>   $assignment_vo -> current_campaign_id
        ));*/
        $query = "SELECT nid FROM node
        LEFT JOIN field_data_field_wisheditem_campaign AS campaign ON node.nid=campaign.entity_id
        LEFT JOIN field_data_field_wisheditem_state AS state on node.nid=state.entity_id
        LEFT JOIN field_data_field_user_team AS team ON node.uid=team.entity_id
        WHERE node.type = 'nodetype_wished_item'
        AND field_wisheditem_state_value = 'open'
        AND campaign.field_wisheditem_campaign_target_id= :campaign_id";
        $result = db_query($query,
            array(':campaign_id' =>   $assignment_vo -> current_campaign_id
            ));
        foreach ($result as $row)
        {
            $nodes[] = node_load($row->nid);
        }
        _scoga_process_assignment_query_results($assignment_vo,$nodes,$assignment_titles,$assignment_paths);
    }

    /**
     * So players can see the wished they created in the play list
     * @param $assignment_vo
     * @param $assignment_titles
     * @param $assignment_paths
     */
    function _scoga_add_own_wishes_to_output($assignment_vo,&$assignment_titles,&$assignment_paths)
    {

        $nodes = array();
        $query = "SELECT nid FROM node
            LEFT JOIN field_data_field_wisheditem_campaign AS campaign ON node.nid=campaign.entity_id
            LEFT JOIN field_data_field_wisheditem_state AS state on node.nid=state.entity_id
            WHERE node.type = 'nodetype_wished_item'
            AND field_wisheditem_state_value = 'open'
            AND campaign.field_wisheditem_campaign_target_id= :campaign_id
            AND uid=:uid
            ";

        $result = db_query($query,
            array(
                ':campaign_id' =>   $assignment_vo -> current_campaign_id,
                ':uid' => $assignment_vo -> user
            ));
        foreach ($result as $row)
        {
            $nodes[] = node_load($row->nid);
        }
        _scoga_process_assignment_query_results($assignment_vo,$nodes,$assignment_titles,$assignment_paths);
    }


    /**
     * Trans team assignments can be set to none in the team field and will then be valid for all teams
     * @param $assignment_vo
     * @param $titles
     * @param $paths
     */
    function _scoga_add_trans_team_assignment_to_output($assignment_vo,&$titles,&$paths)
    {
        $user_team = $assignment_vo ->user_team;
        //get the assignments that are ONLY for the opposing team
        $assignment_vo->user_team = $assignment_vo -> opposing_team;
        $nodes_properties_opposing_team = _scoga_get_assignments_by_type($assignment_vo,$titles,$paths);
        if (!isset($nodes_properties_opposing_team['node']))
        {
            $nodes_properties_opposing_team['node'] = array();
        }
        //get the assignments for ALL teams
        $assignment_vo -> team_ref_field = NULL;
        $assignment_vo ->user_team = $user_team;
        $nodes_properties_all_teams = _scoga_get_assignments_by_type($assignment_vo,$titles,$paths);
        if (!isset($nodes_properties_all_teams['node']))
        {
            $nodes_properties_all_teams['node'] = array();
        }
        //we need the assignments that are ONLY valid for the player's team AND the ones that are valid for ALL teams
        //this means we must subtract the assignments set for the opposing team from assignment set for all teams
        $nodes_properties = array_diff_key($nodes_properties_all_teams['node'],$nodes_properties_opposing_team['node']);
        $nodes = node_load_multiple(array_keys($nodes_properties));
        _scoga_process_assignment_query_results($assignment_vo,$nodes,$titles,$paths);
    }

    /**
     * @param $assignment_vo
     * @param $titles
     * @param $paths
     * @return mixed
     */
    function _scoga_get_assignments_by_type($assignment_vo,&$titles,&$paths)
    {
        $nodeQuery = new EntityFieldQuery;
        if ($assignment_vo -> team_ref_field != NULL) {
            $nodes_properties = $nodeQuery->entityCondition('entity_type', 'node')
                ->entityCondition('bundle', $assignment_vo -> content_type)
                ->fieldCondition($assignment_vo -> campaign_ref_field, 'target_id', $assignment_vo -> current_campaign_id, '=')
                ->fieldCondition($assignment_vo -> team_ref_field, 'target_id', $assignment_vo -> user_team, '=')
                ->propertyCondition('status', '1')
                ->execute();
            return $nodes_properties;
        }
        else {
            $nodes_properties = $nodeQuery->entityCondition('entity_type', 'node')
                ->entityCondition('bundle', $assignment_vo -> content_type)
                ->fieldCondition($assignment_vo -> campaign_ref_field, 'target_id', $assignment_vo -> current_campaign_id, '=')
                ->propertyCondition('status', '1')
                ->execute();
            return $nodes_properties;
        }
    }


    /**
     * @param $assignment_vo
     * @param $nodes
     * @param $titles
     * @param $paths
     */
    function _scoga_process_assignment_query_results($assignment_vo,$nodes,&$titles,&$paths)
    {
        if (sizeOf($nodes)!=0)
        {
            $titles[] = t($assignment_vo -> header);
            $paths[] = '';
            _scoga_set_assignment_titles_and_paths($nodes,$titles,$paths,$assignment_vo -> label);
        }
    }


    /**
     * @param $nodes
     * @param $titles
     * @param $paths
     * @param $label
     */
    function _scoga_set_assignment_titles_and_paths($nodes,&$titles,&$paths,$label)
    {
        foreach($nodes as $node_item)
        {
            $titles[] = t($label).': '.$node_item->title;
            $paths[] = 'node/'.$node_item->nid;
        }
    }

    /**
     * @param $titles
     * @param $paths
     */
    function _scoga_add_POI_confirmations_to_output(&$titles,&$paths)
    {
        global $user;
        $current_campaign_id = scoga_get_campaign_nid_for_time(time());
        $nodeQuery = new EntityFieldQuery;
        $nodes_properties = $nodeQuery->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', 'poi_confirmation')
            ->fieldCondition('field_status', 'value', '0' )
            ->fieldCondition('field_poi_confirm_ref_user', 'target_id', $user->uid )
            ->fieldCondition('field_geochallenge_ref_campaign', 'target_id', $current_campaign_id )
            ->execute();
        if (isset($nodes_properties['node']))
        {
        $nodes = node_load_multiple(array_keys($nodes_properties['node']));
            if (sizeOf($nodes)!=0)
            {
                $titles[] = t('POIs to confirm');
                $paths[] = '';
                _scoga_set_assignment_titles_and_paths($nodes,$titles,$paths,t('POI confirmation'));
            }
        }
    }

    /**
     * @return array
     */
    function scoga_team_score_graph() {
        $returnvalues = array();
        $zerodatapoint = array('date'=>'start','points1'=>0,'points2'=>0);
        $returnvalues['start'] = $zerodatapoint;
        module_load_include( 'api.campaign.inc', 'scoga', 'api/scoga' );
        module_load_include( 'api.team.inc', 'scoga', 'api/scoga' );
        $current_campaign_id = scoga_get_campaign_nid_for_time(time());
        $teams = scoga_get_team_vos_by_campaign_nid($current_campaign_id);
        //load data for team1
        $nid_team1 = scoga_get_campaignteam_nids_by_references($current_campaign_id,$teams[0]->nid);
        $nid_team1 = array_reverse($nid_team1);
        $colour_team1 = '#'.$teams[0]->colour;
        $colour_team2 = '#'.$teams[1]->colour;
        $nodes_team1 = node_load_multiple($nid_team1);
        foreach($nodes_team1 as $node)
        {
            $points = $node->field_campaignteam_totalpoints['und'][0]['value'];
            $date =  date('d/m', $node->created);
            $datapoint = array('date'=>$date,'points1'=>$points);
            $returnvalues[$date]= $datapoint;
        }
        //load data for team2
        //merge data of team1 with team 2
        $nid_team2 = scoga_get_campaignteam_nids_by_references($current_campaign_id,$teams[1]->nid);
        $nid_team2 = array_reverse($nid_team2);
        $nodes_team2 = node_load_multiple($nid_team2);
        foreach($nodes_team2 as $node)
        {
            $points = $node->field_campaignteam_totalpoints['und'][0]['value'];
            $date =  date('d/m', $node->created);

            if (isset($returnvalues[$date]))
            {
                $returnvalues[$date]['points2']=$points;
            }
            else
            {
                $datapoint = array('date'=>$date,'points1'=>$points);
                $returnvalues[$date]= $datapoint;
            }
        }
        //configure graph
        $options_line = array(
            'title' => 'Score per team',
            'backgroundColor' => 'black',
            'fields' => array(
                'points1' => array(
                    'label' => t($teams[0]->title),
                    'colour' => $colour_team1,
                    'enabled' => TRUE,
                ),
                'points2' => array(
                    'label' => t($teams[1]->title),
                    'colour' => $colour_team2,
                    'enabled' => TRUE,
                ),
            ),
            'xAxis' => array(
                'labelField' => 'date',
            ),
            'yAxis' => array(
                'title' => ' ',
            ),
            'data' => $returnvalues,
            'type' => 'line',
        );

        $build['line'] =  array(
            '#theme' => 'visualization',
            '#options' => $options_line,
        );

        return $build;
    }
?>