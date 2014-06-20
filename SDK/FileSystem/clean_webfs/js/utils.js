document.writeln("<script type='text\/javascript' src='../js/prototype.js'></script>");
var lang_setting;
lang_setting = ReadCookie("language");
if (lang_setting == null)
{
    CreateCookie("langSetFlag","0");
    CreateCookie("language","English");
    lang_setting = "English";
}
document.write("<script type=\"text/javascript\", src = \"../js/lang/" + lang_setting + "/lang_str.js\"><\/script>");

var browser_ie = (((navigator.userAgent.indexOf('MSIE') >= 0) && (navigator.userAgent.indexOf('Opera') < 0))?true:false);
var LOGOUT_PAGE = "../cgi/logout.cgi";

/*handle all XML document API due to  browser compatibility */
function GetResponseXML(response)
{
	if(response.length == 0 ||
	  (response.charAt(0) == '<' && 
	  (response.charAt(1) == 'H' || response.charAt(1) =='h') &&
	  (response.charAt(2) == 'T' || response.charAt(2) == 't') &&
	  (response.charAt(3) == 'M' || response.charAt(3) == 'm') &&
	  (response.charAt(4) == 'L' || response.charAt(4) == 'l') &&
	  response.charAt(5) == '>'))
	{
			SessionTimeout();
			return;
	}
	if (window.ActiveXObject){ //ie
		xmlDoc=new ActiveXObject("Microsoft.XMLDOM");
		xmlDoc.async="false";
		xmlDoc.loadXML(response);
	}else if (window.XMLHttpRequest) //Firefox or Safari
	{
		parser=new DOMParser();
		xmlDoc=parser.parseFromString(response,"text/xml");
	}
	
	if(xmlDoc.childNodes[0].nodeName == 'HTML' ||
	   xmlDoc.childNodes[0].nodeName == 'html')
	{
		xmlDoc = null;
	}
	return xmlDoc;
}


/*handle seesion timoue API */
function SessionTimeout()
{
	if(typeof SessionTimeout.record == 'undefined')
	{
		SessionTimeout.record = 0;
	}
	if(SessionTimeout.record == 0)
	{
		++SessionTimeout.record;
		//if (typeof(lang) == 'object' )
			alert(lang.LANG_COMMON_SESSION_TIMEOUT);
		//else 
		//	alert ("Your session has timed out. You will need to open a new session");
		location.href = LOGOUT_PAGE;
	}
}

/*Lauch Java application*/
function GetJNLPRequest (ButtonObj, Flag)
{
	// check if current JRE version is greater than 1.6.0     
	if (deployJava.versionCheck('1.6.0_10+') == false) {                           
		userInput = confirm("You need the latest Java(TM) Runtime Environment. Would you like to update now?");                
		if (userInput == true) {                  
			// Set deployJava.returnPage to make sure user comes back to             
			// your web site after installing the JRE            
			deployJava.returnPage = location.href;                        
			// install latest JRE or redirect user to another page to get JRE from.            
			deployJava.installLatestJRE();         
			}    
	}
	
		
    var host_addr;
    var host_tmp  = location.hostname;
    if((host_tmp.indexOf("[", 0) == -1) && (host_tmp.indexOf(":", 0) != -1) && (host_tmp.indexOf(".", 0) == -1))
        host_addr = "[" + host_tmp + "]:" + location.port;
    else
        host_addr = host_tmp  + ":" + location.port;
	

	if (Flag == 0)
	{
		var JnlpURL = window.location.protocol+"//" + host_addr + "/cgi/url_redirect.cgi?url_name=ikvm" + "&url_type=jwsk";
	}
	else 
	{
		var JnlpURL = window.location.protocol+"//" + host_addr + "/cgi/url_redirect.cgi?url_name=sol" + "&url_type=jwss";
	}

	ButtonObj.disabled=false;  
	ButtonObj.onclick=function()
	{
		deployJava.launch(JnlpURL);
	}
	
}

/*show loading string on web*/
function Loading(enable, text)
{
	text="";
	document.getElementById("loading").innerHTML=enable?text+"<font style='background:#990000; color:#FFFFFF;'>L O A D I N G ...</font>":"";
}

function CheckWord(txt) //the txt only exists letter and number.
{
	re = /\W/;
	if (re.test(txt))
    	return false;
	else
    	return true;
}
/*Check the path for virtual media webpage */
function CheckPath(txt) 
{
	var path = txt;
	var filter = /^\\[a-zA-Z0-9_\$\.\- ]+\\([a-zA-Z0-9_\$\.\- ]+\\*)+(\.[iI][sS][oO]){1}$/;
	
	if(filter.test(path))
		return true;
	else
			 	 
		return false;
}
/*check the user password*/
function CheckPassword(pw)
{
	
    if( (pw.length > 20) || (pw.length < 8) )
    {
        return false;
    }
	else
	{
		var re=/^[^ ]+$/;
		if (re.test(pw)==false)
		{
			//alert(eLang.getSysString("STR_PASSWORD_SPC"));
			return false;
		}
	}
	return true;
}
/*check the user name*/
function CheckUserName(name)
{
    if( (name.length > 16) || (name.length < 4) )
    {
        return false;
    }
    var name1 = new String(name);
    var SpeficCharFilter = /([,; &"<>\\?])/;
    if( name1.match(SpeficCharFilter) )
    {
        return false;
    }
    else
    {
        return true;
    }
}
/*check illegal char*/
function CheckSpeficChar(str)
{
    var str1 = new String(str);
    var SpeficCharFilter = /([,; &"<>\\=$#*!@~`%^])/;
    if( str1.match(SpeficCharFilter) )
    {        
        return false;
    }
    else
    {
        return true;
    }
}   
/*check number*/
function CheckNumber(n)
{
	return typeof n == 'number' && isFinite(n);
}
/*check if IP address is legal*/
function CheckIP(ipAddr)
{
	var addr = (new String(ipAddr)).split(".");
	if(addr.length != 4)
	{
		return false;
	}
	for( var i=0; i<4 ;i++)
	{
		if( isNaN(addr[i]) || addr[i]=="" || addr[i] < 0 || addr[i] > 255 ||addr[i].length>3)
		{
			return false;
		}
	}

	delete addr;
	return true;
}
/*check the file ext name*/
function CheckExtName(str, lookfor)
{
    var strlen = str.length;
    var lookforlen = lookfor.length;
                                                                                                                         
    var lookforptr = lookforlen;
    while( lookforptr > 0 )
    {
            if( str.charAt(strlen-lookforptr) != lookfor.charAt(lookforlen-lookforptr) )
                    return false;
            lookforptr--;
    }
    return true;
}

function Trim(str)
{ 
	return str.replace(/^\s+|\s+$/g,"");
}
/*for event log */
function GetSubString(str,start,end)
{
	var ori = str;
	var res =0x00;
	for(var i=start;i>=end;i--)
		res = res | (1 << i);
	return (ori & res);
}
/* for user page*/
function GetVars(str)
{
	url=location.search.substring(1); 
	var parameterList=url.split("&");        
	for (var i=0;i<parameterList.length;i++)
	{
		 parameter=parameterList[i].split("="); 		 
		 if (parameter[0] == str)          
		 	return (decodeURIComponent(parameter[1]));		 	        
	}	
}

function ToLocale(str)
{
	var newstr = str.substring(0,7)+str.substring(15)+str.substring(6,16);
	var CardDate = new Date(newstr + " GMT");
	return ( CardDate.toLocaleString() );

}

/* Handle  cookies API */
function CreateCookie(name, value)
{
	top.document.cookie = name+"="+value+"; path=/";
}

/* Clear Session Cookie */
function getcookieval(offset)
{
   	var endstr = document.cookie.indexOf(";", offset);
   	if (endstr == -1)
    	endstr = document.cookie.length;
   	return unescape(document.cookie.substring(offset, endstr));
}
 
function ReadCookie(name) // getcookie(name)
{
   	var arg = name + "=";
   	var alen = arg.length;
   	var clen = document.cookie.length;
   	var i = 0;
   	while (i < clen)
   	{
       	var j = i + alen;
       	if (document.cookie.substring(i, j) == arg)
       		return getcookieval(j);
       	i = document.cookie.indexOf(" ", i) + 1;
       	if (i == 0) break;
   	}
   	return null;
}
 
function EraseCookie(name) // deletecookie(name)
{   
   	var exp = new Date();
   	exp.setTime(exp.getTime() - 1); // expire immediately
   	var cval = ReadCookie(name);
   	if(cval != null) document.cookie = name + "=" + cval + "; expires=" + exp.toGMTString() + "; path=/;";
   	return;
}
//Sensor API    
function ToSigned(Num, signedbitB)
{
	
	if(signedbitB > 0)
	{
		
		/* positive */
		if( ( Num%(0x01<<signedbitB)/(0x01<<(signedbitB-1)) ) < 1 )
		{
			return Num%(0x01<<signedbitB-1);
		}
		/* negative */
		else
		{
			var temp = (Num%(0x01<<signedbitB-1)) ^ ((0x01<<signedbitB-1)-1);
			return (-1-temp);
		}
		
	}
	
	else
	{
		return Num;
	}
	
}

    
function SensorFunc(raw_data, m, b, rb)
{
	var sensor_data;
	
	var M_raw, M_data;
	var B_raw, B_data;
	var Km_raw, Km_data;
	var Kb_raw, Kb_data;


	/* change sequense of lsb and msb into 10b char */ 
	M_raw = ((parseInt(m,16)&0xC0) << 2) + ( parseInt(m,16) >> 8);
	B_raw = ((parseInt(b,16)&0xC0) << 2) + ( parseInt(b,16) >> 8);


	Km_raw = parseInt(rb,16) >> 4;
	Kb_raw = (parseInt(rb,16) & 0x0F);
	
	M_data = ToSigned(M_raw, 10);
	B_data = ToSigned(B_raw, 10);
	Km_data = ToSigned(Km_raw, 4);
	Kb_data = ToSigned(Kb_raw, 4);
		
	sensor_data = (M_data*parseInt(raw_data, 16) + B_data*Math.pow(10, Kb_data)) * Math.pow(10,Km_data);

	return sensor_data;
	
}
function ShowDiscStateAPI( Sensor_Type, sensor_d )
{
	var State_String = "";
	/*
	sensor_d is the sensor reading value
	We convert sensor_d to sensor specific offset and show its corresponding event string.
	*/
	ShowDiscStateAPI.SensorHealth = "bgcolor=red";
	if( Sensor_Type == "05" )
	{		
		if(sensor_d == 0){
			State_String += 'OK';
			ShowDiscStateAPI.SensorHealth = "bgcolor=green";
		}
		if(parseInt((sensor_d/1), 10) % 2)
			State_String += 'General Chassis Intrusion. ';
		if(parseInt((sensor_d/2), 10) % 2)
			State_String += 'Drive Bay intrusion. ';
		if(parseInt((sensor_d/4), 10) % 2)
			State_String += 'I/O Card area intrusion. ';
		if(parseInt((sensor_d/8), 10) % 2)
			State_String += 'Prosessor area intrusion. ';
		if(parseInt((sensor_d/16), 10) % 2)
			State_String += 'LAN Leash Lost. ';
		if(parseInt((sensor_d/32), 10) % 2)
			State_String += 'Unauthorized dock. ';
		if(parseInt((sensor_d/64), 10) % 2)
			State_String += 'Fan area intrusion. ';
			
		return State_String;
	}
	else if( Sensor_Type == "08" )
	{
		ShowDiscStateAPI.SensorHealth = "bgcolor=green"; //linda fix health icon stays as critical(red) as power supply not present
		//if(!(parseInt((sensor_d/1), 10) % 2) && parseInt((sensor_d/2), 10) % 2)	//bit0 = 0 and bit1 = 1
		if(!(parseInt((sensor_d/1), 10) % 2) && !(parseInt((sensor_d/2), 10) % 2))	//bit0 = 0 and bit1 = 0
		{
			//State_String += 'Not Present or Failure detected. ';
			State_String += 'Not Present. ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=white";
		}
		else
		{
			if(parseInt((sensor_d/1), 10) % 2){
				State_String += 'Presence detected. ';
				ShowDiscStateAPI.SensorHealth = "bgcolor=green";
			}
			if(parseInt((sensor_d/2), 10) % 2)
			{
				State_String += 'Power Supply Failure detected. ';
				ShowDiscStateAPI.SensorHealth = "bgcolor=red";
			}
			if(parseInt((sensor_d/4), 10) % 2)
			{
				State_String += 'Predictuve Failure. ';
				ShowDiscStateAPI.SensorHealth = "bgcolor=red";
			}
			if(parseInt((sensor_d/8), 10) % 2)
			{
				State_String += 'Power Supply input lost (AC/DC). ';
				ShowDiscStateAPI.SensorHealth = "bgcolor=red";
			}
			if(parseInt((sensor_d/16), 10) % 2)
			{
				State_String += 'Power Supply input lost or out-of-range. ';
				ShowDiscStateAPI.SensorHealth = "bgcolor=red";
			}
			if(parseInt((sensor_d/32), 10) % 2)
			{
				State_String += 'Power Supply input out-of-range, but present. ';
				ShowDiscStateAPI.SensorHealth = "bgcolor=red";
			}
			if(parseInt((sensor_d/64), 10) % 2)
			{
	  			State_String += 'Configuration error. ';
				ShowDiscStateAPI.SensorHealth = "bgcolor=red";
			}
		}
  			
  		return State_String;
	}
	/* Tony, 10/23/2012 add battery backup power { */
	else if( Sensor_Type == "29" )
	{
		ShowDiscStateAPI.SensorHealth = "bgcolor=green"; //linda fix health icon stays as critical(red) as power supply not present

		if(parseInt((sensor_d/1), 10) % 2){
			State_String += 'Battery low (predictive failure). ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=red";
		}
		if(parseInt((sensor_d/2), 10) % 2)
		{
			State_String += 'Battery failed. ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=red";
		}
		if(parseInt((sensor_d/4), 10) % 2)
		{
			State_String += 'Battery presence detected. ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=green";
		}
			
  		return State_String;
	}
	/* Tony, 10/23/2012 add battery backup power } */	
//Linda HDD begin ---
	else if( Sensor_Type == "0d" )
	{
		ShowDiscStateAPI.SensorHealth = "bgcolor=green";
		if(parseInt((sensor_d/1), 10) % 2){
			State_String += 'Drive Presence. ';
			//State_String += 'Drive Normal. ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=green";
		}
		if(parseInt((sensor_d/2), 10) % 2)
		{
			State_String += 'Drive Fault. ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=red";
		}
		if(parseInt((sensor_d/4), 10) % 2)
		{
			State_String += 'Predictuve Failure. ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=red";
		}
		if(parseInt((sensor_d/8), 10) % 2)
		{
			State_String += 'Hot Spare. ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=red";
		}
		if(parseInt((sensor_d/16), 10) % 2)
		{
			State_String += 'Consistency Check/ Parity Check in progress. ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=red";
		}
		if(parseInt((sensor_d/32), 10) % 2)
		{
			State_String += 'In Critical Array. ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=red";
		}
		if(parseInt((sensor_d/64), 10) % 2)
		{
  			State_String += 'In Failed Array. ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=red";
		}
		if(parseInt((sensor_d/128), 10) % 2)
		{
  			State_String += 'Rebuid/Remap in progress. ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=red";
		}
		if(parseInt((sensor_d/256), 10) % 2)
		{
  			State_String += 'Rebuid/Remap Aborted. ';
			ShowDiscStateAPI.SensorHealth = "bgcolor=red";
		}
  			
  		return State_String;
	}
//Linda HDD end ---
	else if( Sensor_Type == "c0" )
	{
		//linda modified for cpu temp reading


		if(sensor_d==0)
		{
			State_String += 'Low';
			ShowDiscStateAPI.SensorHealth = "bgcolor=green";
		}
		else if(sensor_d==1){
			State_String += 'Medium';
			ShowDiscStateAPI.SensorHealth = "bgcolor=yellow";
		}
		else if(sensor_d==2)
			State_String += 'High';
		else if(sensor_d==4)
			State_String += 'Over Heat';
		else if(sensor_d==8){
			State_String += 'Uninstall';
			ShowDiscStateAPI.SensorHealth = "bgcolor=white";
		}
  		else 
  			State_String += 'Not Present!';

/*
		if(parseInt((sensor_d/1), 10) % 2){
			State_String += 'Low';
			ShowDiscStateAPI.SensorHealth = "bgcolor=green";
		}
		else if(parseInt((sensor_d/2), 10) % 2){
			State_String += 'Medium';
			ShowDiscStateAPI.SensorHealth = "bgcolor=yellow";
		}
		else if(parseInt((sensor_d/4), 10) % 2)
			State_String += 'High';
		else if(parseInt((sensor_d/16), 10) % 2)
			State_String += 'Over Heat';
		else if(parseInt((sensor_d/128), 10) % 2){
			State_String += 'Uninstall';
			ShowDiscStateAPI.SensorHealth = "bgcolor=white";
		}
  		else 
  			State_String += 'Not Present!';
*/
  			
  		return State_String;
	}
	else if( Sensor_Type == "c2" )
	{
		if(sensor_d == 0){
			State_String += 'OK';
			ShowDiscStateAPI.SensorHealth = "bgcolor=green";
		}
		if(parseInt((sensor_d/1), 10) % 2)
			State_String += 'None of The Above Fault';
		if(parseInt((sensor_d/2), 10) % 2)
			State_String += 'CML Fault';
		if(parseInt((sensor_d/4), 10) % 2)
			State_String += 'Over Temperature Fault';
		if(parseInt((sensor_d/8), 10) % 2)
			State_String += 'Under Voltage Fault';
		if(parseInt((sensor_d/16), 10) % 2)
			State_String += 'Over Current Fault';
		if(parseInt((sensor_d/32), 10) % 2)
			State_String += 'Over Ovltage Fault';
		if(parseInt((sensor_d/64), 10) % 2)
  			State_String += 'PS On/Off';
  		if(parseInt((sensor_d/128), 10) % 2)
  			State_String += 'Device Busy';	
  		return State_String;
	}
}	
//Convert integer to hex string
function IntegerToHexString(Num)
{
	var Value = String.fromCharCode(Num);  
	var Value1 = Value.charCodeAt(0);
 	return  Value1.toString(16).toUpperCase();
}//process sensor function
var SensorReadingScale=1000
function SensorFormula(node,Idx,SensorTableArray)
{
  		var SensorType = node.getAttribute("STYPE");
  	 	var SensorReadingObj = node.getAttribute("READING");
  	 	var RawReading = SensorReadingObj.substr(0, 2);
		var Option = parseInt(node.getAttribute("OPTION"), 16);
		var SFormula = node.getAttribute("L");
  	 	var UnitType1 = parseInt(node.getAttribute("UNIT1"), 16);
  	 	var UnitType = parseInt(node.getAttribute("UNIT"), 16);
  	  	var Unit;
  	 	var AnalogDataFormat = UnitType1 >> 6;			
  	 	var UNR, UC, UNC, LNC, LC, LNR;
  	 	var ReadingDataFormat;
  	 	
  	 	SensorFormula.NeedCompare = 0;
  	 	
  	 	switch(UnitType)
  	 	{ 	 						
			case 0x01:    
				Unit = " degrees C"; 
				break;
			case 0x02:	
				Unit = " degrees F";
				break;
			case 0x03:	
				Unit = " degrees K";
				break;
			case 0x04:
				Unit = " Volts";
				break;
			case 0x05:
				Unit = " Amps";
				break; 
			case 0x06:
				Unit = " Watts";
				break;
			case 0x07:
				Unit = " Joules";
				break;
			case 0x12:
				Unit = " R.P.M";					
				break; 
			case 0x13:
				Unit = " Hz";
				break; 
			default:
				break;
		}
  		SensorFormula.Unit = Unit;
  		// 2's complement
		if(AnalogDataFormat == 0x02 ) 
		{
			ReadingDataFormat = ToSigned(parseInt(RawReading, 16), 8).toString(16);
			UNR =  ToSigned(parseInt(node.getAttribute("UNR"), 16), 8).toString(16);
			UC =   ToSigned(parseInt(node.getAttribute("UC"), 16), 8).toString(16);
			UNC =  ToSigned(parseInt(node.getAttribute("UNC"), 16), 8).toString(16);
			LNC =  ToSigned(parseInt(node.getAttribute("LNC"), 16), 8).toString(16);
			LC =   ToSigned(parseInt(node.getAttribute("LC"), 16), 8).toString(16);
			LNR =  ToSigned(parseInt(node.getAttribute("LNR"), 16), 8).toString(16);
		}
		else 
		{
			ReadingDataFormat = RawReading;
			UNR = node.getAttribute("UNR");
			UC = node.getAttribute("UC");
			UNC = node.getAttribute("UNC");
			LNC = node.getAttribute("LNC");
			LC = node.getAttribute("LC");
			LNR = node.getAttribute("LNR");
		}
		var AfterFuncSensorReading = parseFloat( SensorFunc(ReadingDataFormat, node.getAttribute("M"), node.getAttribute("B"), node.getAttribute("RB")), 10)
		var AfterFuncSensorUNR = parseFloat( SensorFunc(UNR, node.getAttribute("M"), node.getAttribute("B"), node.getAttribute("RB")), 10);
  		var AfterFuncSensorUC  = parseFloat( SensorFunc(UC, node.getAttribute("M"), node.getAttribute("B"), node.getAttribute("RB")), 10) ;
  		var AfterFuncSensorUNC = parseFloat( SensorFunc(UNC, node.getAttribute("M"), node.getAttribute("B"), node.getAttribute("RB")), 10);
  		var AfterFuncSensorLNC = parseFloat( SensorFunc(LNC, node.getAttribute("M"), node.getAttribute("B"), node.getAttribute("RB")), 10);
  		var AfterFuncSensorLC  = parseFloat( SensorFunc(LC, node.getAttribute("M"), node.getAttribute("B"), node.getAttribute("RB")), 10) ;
  		var AfterFuncSensorLNR = parseFloat( SensorFunc(LNR, node.getAttribute("M"), node.getAttribute("B"), node.getAttribute("RB")), 10);
  		
  		// Ignore on reading
  		if(!(Option & 0x40))
  		{
			if ( typeof(SensorTableArray) == 'object' )
			{
				SensorTableArray[Idx][2] = "Not Present!";
				SensorTableArray[Idx][9] = "bgcolor=white";
			}
		}
		else
		{	
	  	 	if(SFormula == "00")
	  	 	{	 	 			
	  	 		var SFunction = function(val) 
	  	 		{
	  	 			return parseInt( val*SensorReadingScale, 10)/SensorReadingScale;
	  	 		}
				if(RawReading == '00' && Option != 0x00 && SensorType == '04')
				{
					var SensorReading = 0;
					if ( typeof(SensorTableArray) == 'object' )
	  	 				SensorTableArray[Idx][2] = "0 " + Unit;	
	  	 			SensorFormula.NeedCompare = 1;
	  	 			SensorFormula.SensorReading = SensorReading;
	  	 		}
				else if((node.getAttribute("READING") == '      ') || (RawReading == '00'))
			    {
	  	 			if ( typeof(SensorTableArray) == 'object' )
	  	 				SensorTableArray[Idx][2] = "Not Present!";	 					
	  	 		}
	  	 		/* 	linear_reading	*/
	  	 	    else
	  	 		{  	 						
	  	 			var SensorReading = parseInt(AfterFuncSensorReading*SensorReadingScale, 10)/SensorReadingScale;
	  	 			if ( typeof(SensorTableArray) == 'object' )           		
				    	SensorTableArray[Idx][2] = SensorReading + " " + Unit;	
	                SensorFormula.NeedCompare = 1;
	                SensorFormula.SensorReading = SensorReading;
	  	 		}
			}
			// linear function = 1/x				
		    else if(SFormula == "07") 
			{	 				
	  	 		var SFunction = function(val) 
	  	 		{
	  	 			 return parseInt(SensorReadingScale/val, 10)/SensorReadingScale;
	  	 		}
	
				if(RawReading == '00' && Option != 0x00 && SensorType == '04')
				{
					SensorFormula.SensorReading = 0;
	  	 			if ( typeof(SensorTableArray) == 'object' )
	  	 				SensorTableArray[Idx][2] = "0 " + Unit;	
	  	 			SensorFormula.NeedCompare = 1;  
	  	 		}
				else if((node.getAttribute("READING") == '      ') || (RawReading == 'ff') || (RawReading == '00'))
				{
	  				if ( typeof(SensorTableArray) == 'object' )
	  					SensorTableArray[Idx][2] = "Not Present!";	 				
	  	 		}
	  	 		/* 	linear_reading	*/	
	  	 		else
	  	 		{
	  	 			var SensorReading = parseInt( SensorReadingScale/AfterFuncSensorReading, 10)/SensorReadingScale;           		
	                if ( typeof(SensorTableArray) == 'object' )
	                	SensorTableArray[Idx][2] = SensorReading + " " + Unit;
	 				SensorFormula.NeedCompare = 1;
	 				SensorFormula.SensorReading = SensorReading;
	  	 	    }
			}
			// linear function = sqr(x)
	 		else if(SFormula == "08") 
			{	 				
	  	 		var SFunction = function(val) 
	  	 		{
	  	 			 return parseInt(parseFloat( Math.pow(val,2), 10) * SensorReadingScale, 10) / SensorReadingScale;
	  	 		}
					
				if(RawReading == '00' && Option != 0x00 && SensorType == '04')
				{
					SensorFormula.SensorReading = 0;
	  	 			if ( typeof(SensorTableArray) == 'object' )
	  	 				SensorTableArray[Idx][2] = "0 " + Unit;	
	  	 			SensorFormula.NeedCompare = 1;  
	  	 		}
				else if((node.getAttribute("READING") == '      ') || (RawReading == 'ff') || (RawReading == '00'))
				{
	  				if ( typeof(SensorTableArray) == 'object' )
		  				SensorTableArray[Idx][2] = "Not Present!";	 				
	  	 		}
	  	 		/* 	linear_reading	*/	
	  	 		else
	  	 		{
	  	 		   	var SensorReading = parseInt(parseFloat( Math.pow(AfterFuncSensorReading,2), 10) * SensorReadingScale, 10) / SensorReadingScale;               		
	                if ( typeof(SensorTableArray) == 'object' )
	                	SensorTableArray[Idx][2] = SensorReading + " " + Unit;
	                SensorFormula.SensorReading = SensorReading;
	 				SensorFormula.NeedCompare = 1;
	  	 		}
			}
			SensorFormula.SensorUNR = SFunction( AfterFuncSensorUNR);
	  		SensorFormula.SensorUC  = SFunction( AfterFuncSensorUC);
	  	 	SensorFormula.SensorUNC = SFunction( AfterFuncSensorUNC);
	  	 	SensorFormula.SensorLNC = SFunction( AfterFuncSensorLNC);
	  	 	SensorFormula.SensorLC  = SFunction( AfterFuncSensorLC);
	  	 	SensorFormula.SensorLNR = SFunction( AfterFuncSensorLNR);
			if ( typeof(SensorTableArray) == 'object' )
			{
				SensorTableArray[Idx][8] = SensorFormula.SensorUNR;
	        	SensorTableArray[Idx][7] = SensorFormula.SensorUC;
	        	SensorTableArray[Idx][6] = SensorFormula.SensorUNC;
	        	SensorTableArray[Idx][5] = SensorFormula.SensorLNC;
	        	SensorTableArray[Idx][4] = SensorFormula.SensorLC;
	        	SensorTableArray[Idx][3] = SensorFormula.SensorLNR;			
	        }
		}	
}
function isIpv6Addr(ip)
{
	return ( ip.indexOf(':') >= 2 )	
}
String.prototype.trim = function()
{
		return this.replace(/^\s+|\s+$/g,"");
}

function HandleXMLResponse(ResponseFunc,Request)
{
			
	if (Request.readyState == 4 && Request.status == 200)
	{
		var response = Request.responseText.replace(/^\s+|\s+$/g,"");
		var xmldoc	 = GetResponseXML(response);
		if(xmldoc == null)
		{
			SessionTimeout();
			return;
		}
		if (typeof (ResponseFunc) == "function")
		{	
			ResponseFunc(xmldoc);
		}
	}
}
function port_number_check(port){
	if(!parseInt(port) || parseInt(port) < 0 || parseInt(port) > 65535  || isNaN(port))
		return 0;
	return 1;
}
function Get_IKVM_VM_Status(Callbackfunc,flag)
{	
	var url = '/cgi/ipmi.cgi';
	var pars = 'GETPORTSINFO.XML=(0,0)&time_stamp='+(new Date()) ;
	var myAjax = new Ajax.Request(
                 url,
                 {method: 'post',parameters:pars, onComplete: GetPortConfigHandler}//reigister callback function
                 );

	function GetPortConfigHandler(originalRequest)
	{
		if (originalRequest.readyState == 4 && originalRequest.status == 200)
		{
	
			var response = originalRequest.responseText.replace(/^\s+|\s+$/g,"");
			var xmldoc = GetResponseXML(response);
			if(xmldoc == null)
			{
				SessionTimeout();
				return;
			}
			var IPMIRoot = xmldoc.documentElement;
			var PortInfo = IPMIRoot.getElementsByTagName("PORT_INFO");
	        
			if (typeof (Callbackfunc) == 'function' )
			{
				if (flag == 0)
					Callbackfunc(PortInfo[0].getAttribute("IKVM_SERVICE")=="1");
				else 
					Callbackfunc(PortInfo[0].getAttribute("VM_SERVICE")=="1");
			}
		}
	}
}
function GetIKVMStatus(CallbackFunc) {
	return Get_IKVM_VM_Status(CallbackFunc,0);
}
function GetVMStatus(CallbackFunc) {
	return Get_IKVM_VM_Status(CallbackFunc,1);
}
function CheckInputValue(input)
{
    var SpeficCharFilter = /[,;&'"<>\\=$|^?*~`()\[\]\{\}]/;
    if( input.value.match(SpeficCharFilter) )
    {        

		alert("The special characters are not allowed!\n ,; &\'\"<>\\=$|^?*~`()[]{}");
		window.setTimeout( function(){input.focus(); },0);
        return false;
    }
    else
        return true;
}
/*check E-mail address*/
function CheckEMAIL(input)
{
	var filter =  /^([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9])+$/;

	if (input.value == "" || input.value == "NULL" )
		return true;

	if ( input.value.match(filter ) )
	{
		return true;
	}

	alert ("Please correct E-Mail format!")
	window.setTimeout( function(){input.focus(); },0);
	return false;
}
function CheckPortNumber(input){

	val = port_number_check(input.value);
	if (val == 0){
		alert("The port number must be between 1 and 65535");
		window.setTimeout( function(){input.focus(); },0);
        return false;
	}
	return true;
}
function CheckValue(input,min,max){

	val = input.value;
	if (input.value == "" || input.value == "NULL" )
		return true;

	if( parseInt(val) < min || parseInt(val) > max  || isNaN(val)){
		alert("The value must be between " + min + " and " + max);
		window.setTimeout( function(){input.focus(); },0);
        return false;
	}
	return true;
}
/*check if IP address is legal*/
function CheckIPAddr(input)
{
	var addr = (new String(input.value)).split(".");

	if(addr.length != 4)
	{
		return false;
	}
	for( var i=0; i<4 ;i++)
	{
		if( isNaN(addr[i]) || addr[i]=="" || addr[i] < 0 || addr[i] > 255 ||addr[i].length>3)
		{
			alert("Please correct IP format!");
			window.setTimeout( function(){input.focus(); },0);
			delete addr;
			return false;
		}
	}

	delete addr;
	return true;
}
function CheckDomainName(input){


	var filter 			 =  /([\w+]\.)+\w{1,}$/;
    var SpeficCharFilter = /[,;&'"<>\\=$|^?*~`()\[\]\{\}]/;

	if ( input.value.match(filter ) ){
		if(!input.value.match(SpeficCharFilter) )
		return true;
	}
	alert("Please correct IP format!");
	window.setTimeout( function(){input.focus(); },0);
	return false;
}