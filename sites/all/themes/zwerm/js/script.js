/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - http://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {

    //for hiding the labels in the login screen username and password fields
    //based on http://blog.stannard.net.au/2011/01/07/creating-a-form-with-labels-inside-text-fields-using-jquery/
    $(document).ready(function(){
        //hiding the address bar on iOS devices
        window.scrollTo(0,1);

        // Find each of our input fields
        //var fields = $("form#user-login input name,form#user-login input pass");
        var fields = $("#edit-name,#edit-pass");

        // If a field gets focus then hide the label
        // (which is the previous element in the DOM).
        fields.focus(function(){
            $(this).prev().hide();
        });

        // If a field loses focus and nothing has
        // been entered in the field then show the label.
        fields.blur(function(){
            if (!this.value) {
                $(this).prev().show();
            }
        });

        //if some browser feature has already added a value to the field,
        //like retrieved credentials from keychain, hide the labels
        fields.each(function(){
            if (this.value){
                $(this).prev().hide();
            }
        });

        fields.change(function(){
            if (this.value){
                $(this).prev().hide();
            }
        });

        jQuery(".collapsible").hide();
        jQuery(".list_header_geochallenge").click(function()
        {
            jQuery(".collapsible").hide();
            jQuery(".geochallenge").slideToggle(500);

        });

        jQuery(".list_header_wish").click(function()
        {
            jQuery(".collapsible").hide();
            jQuery(".wish").slideToggle(500);
        });

        jQuery(".list_header_event").click(function()
        {
            jQuery(".collapsible").hide();
            jQuery(".event").slideToggle(500);
        });

        jQuery(".list_header_question").click(function()
        {
            jQuery(".collapsible").hide();
            jQuery(".question").slideToggle(500);
        });
        jQuery(".list_header_poi_confirmation").click(function()
        {
            jQuery(".collapsible").hide();
            jQuery(".POI_conf").slideToggle(500);
        });
        jQuery(".list_header_active_generic_assignment").click(function()
        {
            jQuery(".collapsible").hide();
            jQuery(".generic_assignment").slideToggle(500);
        });
        jQuery(".list_header_wishes_user").click(function()
        {
            jQuery(".collapsible").hide();
            jQuery(".wish_by_user").slideToggle(500);
        });
        jQuery(".list_header_writein_question").click(function()
        {
            jQuery(".collapsible").hide();
            jQuery(".writein_question").slideToggle(500);
        });
        jQuery("#menu_container").click(function()
        {
            jQuery('html, body').animate({scrollTop:0}, 'fast');
            jQuery("#main-menu").slideToggle(500);
            jQuery("#footer").slideToggle(500);
        });
    });

    $(document).ready(function(){
        jQuery(".help").click(function()
        {
            $("#dialog").dialog();
        });
    });

    $(document).ready(function(){
    $('.geochallenge-add-poi').show();
    $('.geochallenge-add-poi').click(function()
    {
      //jQuery('html, body').animate({scrollTop:0}, 'fast');
      jQuery(".geochallenge-add-poi-instructions").slideDown('fast');

      jQuery('html, body').animate({
          scrollTop: $(".geochallenge-add-poi").offset().top-202
        }, 'fast');
      $(".geochallenge-add-poi").addClass('button-pressed');

      return false;
    });

    jQuery(".geochallenge-confirm-poi").click(function()
    {
      var r = window.confirm("Ben je zeker dat je dit punt wil goedkeuren?");
      if (r==true)
      {
        $(".geochallenge-buttons").hide();
        $(".rules-link-poi_confirm_accept a").trigger('click');
        $(".geochallenge-confirmation-details").hide();
        $(".geochallenge-back-button").show();
        alert('Bedankt! Je keuze wordt geregistreerd en van zodra je medespeler ook diens keuze heeft gemaakt, krijg je (hopelijk!) je punten toegekend!');
      }
      return(false);
    });

    jQuery(".geochallenge-reject-poi").click(function()
    {
      var r = window.confirm("Ben je zeker dat je dit punt wil afkeuren?");
      if (r==true)
      {
        $(".geochallenge-buttons").hide();
        $(".rules-link-poi_confirm_reject a").trigger('click');
        $(".geochallenge-confirmation-details").hide();
        $(".geochallenge-back-button").show();
        alert('Bedankt! Je keuze werd geregistreerd en van zodra je medespeler ook diens keuze heeft gemaakt, krijg je (hopelijk!) je punten toegekend!');
      }
      return(false);
    });

  });


})(jQuery, Drupal, this, this.document);

function li_mousedown(url,element)
{
    element.className="selectedItem";
    document.location = url;
}



