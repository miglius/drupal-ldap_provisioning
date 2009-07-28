// $Id$

/**
 * Behaviours are bound to the Drupal namespace.
 */
Drupal.behaviors.ldapprov = function(context) {
  $('#edit-test').click(function(event) {
    $('#test-message').hide();
    $('#test-spinner').show();
    var url = window.location.href + '/test';
    $.post(url, { sid: $('#edit-ldapprov-server').val(), binddn: $('#edit-ldapprov-dn').val(), bindpw: bindpw = $('#edit-ldapprov-pass').val(), bindpw_clear: bindpw_clear = $('#edit-ldapprov-pass-clear').val() },
      function(data){
        $('#test-spinner').hide();
        $('#test-message').show().removeClass('status error').addClass(data.status ? 'status' : 'error').html(data.message);
      }, "json");
    return false;
  });
}

