/* 
 * LWbase.js 
 * Base library for the LightWeigthComponents project
 * (https://sourceforge.net/projects/lwcomponents/)
 *
 * @copyright (C) 2005 Luca Mariano <luca.mariano@email.it>  
 * @license GPL [http://www.fsf.org/licensing/licenses/gpl.txt]
 * @author Luca Mariano <luca.mariano@email.it> 
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307, USA. 
 *
 */
function emptySelect(selectName)
{
    var objElement = document.getElementById(selectName);
    for (i=objElement.length;i>=0;i--)
    {
        objElement.options[i]=null;
    }
}

function fillSelect(selectName,values,defaultChoice)
{
    var objElement = document.getElementById(selectName);
    emptySelect(selectName);
    if (defaultChoice)
    {
        objElement[objElement.length] = new Option("choose...", "");
    }
    
    for (i=0; i< values.length; i++)
    {
        objElement[objElement.length] = new Option(values[i][1], values[i][0]);	
    }
}

function isArray(obj) {
   if  (typeof(obj) == 'undefined')
       return false;
   if (obj.constructor.toString().indexOf("Array") == -1)
      return false;
   else
      return true;
}

function array_key_exists(arr,key)
{

		return typeof(arr[key]) != 'undefined';
}

function isString(obj) {
   if  (typeof(obj) == 'undefined')
       return false;
   if (obj.constructor.toString().indexOf("String") == -1)
      return false;
   else
      return true;
}

function IsNumeric(strString)
   //  check for valid numeric strings	
   {
   var strValidChars = "0123456789.,";
   var strChar;
   var blnResult = true;

   if (strString.length == 0) return false;

   //  test strString consists of valid characters listed above
   for (i = 0; i < strString.length && blnResult == true; i++)
      {
      strChar = strString.charAt(i);
      if (strValidChars.indexOf(strChar) == -1)
         {
         blnResult = false;
         }
      }
   return blnResult;
   }
   

function getCheckedValue(radioName) {
var inputs = document.getElementsByTagName('input');
for (var i=0;i<inputs.length;i++)
{
    if (inputs[i].name.substr(0,radioName.length) == radioName )
        {
        var element = inputs[i];
        if (element.checked)
            return element.value;
         }
}
return false;

}

function compareNums(a,b) {
	a=Number(a);
	b=Number(b);
    //a= Number(a.replace(/,/,''));
    //b= Number(b.replace(/,/,''));

     if (a < b) 
         return -1;
     if (a > b) 
         return 1;
    return 0;
}

function compareString(a,b) {
     if (a < b) 
         return -1;
     if (a > b) 
         return 1;
    return 0;
}









