/*
 * LWdatagrid.js
 * This the datagrid widget of the LightWeigthComponents project
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

var LWdatagrid_idx = 0;

//------------------------------------------------------------
//the variables which is used by outside function
//to operate the LWdatagrid.
//------------------------------------------------------------
var rowSel = null;
var rowSelEnable = 0;
/*
 * LWdatagrid
 * Ctor
 * optionally accepts the columns definition array
 *
 */
function LWdatagrid(doc)
{
    this.name = 'LWdatagrid_'+LWdatagrid_idx++;
    this.columns = new Array();
    this.data = new Array();
//    this.selectObj = null;
    this.attributeList = new Array();
    this.doc = doc;
    this.sortCol = null;
    this.descending = [];// = false;
    this.instanceName = null;
    
	//this.rowSel[];
    function _removeRow(key)
    {
//         for (var i=0; i<this.selectObj.length; i++)
//         {
//            if (this.selectObj.options[i].value == key )
//            {
//                this.selectObj.remove(i);
//                this.data[key] = null;
//            }
//          }

    }

    function _addRow(row)
    {
		row.shift();
	    var trObj = this.docObj.createElement("TR");
	    this.tbodyObj.appendChild(trObj);
	    trObj.className = 'normal';
	    trObj.style.height = '19px'; ////doris, work fine on ie, cannot work on firefox
	    trObj.onclick = function()
	    {
	    	if(rowSelEnable)
	    	{
		    	if(rowSel!= null)
		    		rowSel.className = 'normal';
		    	rowSel = trObj;  	
		        trObj.className = 'selected';
		        trObj.focus();
	    	}
	    }
	    for (var j=0;j<  this.columns.length;j++)
	    {
	    	if(row[j].toString().search('bgcolor') != -1) //fill bgcolor into table (ex: bgcolor=red, bgcolor=green...)
	    	{
	    		var td = trObj.insertCell(j);
		    	td.style.textAlign = this.alignArray[j];
		    	td.style.paddingRight = '1em';
	        	td.style.paddingLeft = '1em';
	        	if(row[j].substring(8) != 'white')
	        		td.style.background = row[j].substring(8); 		    	
		    }
			else 
			{
	    	var td = trObj.insertCell(j);
	    	td.style.textAlign = this.alignArray[j];
	    	td.style.paddingRight = '1em';
        	td.style.paddingLeft = '1em';
	    	td.innerHTML = row[j];
	    }
	
	    }
    }

    function _selectAll()
    {
//        for (var i=this.selectObj.length-1;i>=0;i--)
//        {
//        this.selectObj.options[i].selected=true;
//        }
    }

    function _getSortFunc()
    {
        // try to guess the right column type...
        var exampleData = this.data[0][this.sortCol];
        if (IsNumeric(exampleData))
        {
            return compareNums;

        } else {
            return compareString;
        }
     }


    this._addRow = _addRow;
    this._removeRow = _removeRow;
    this._selectAll = _selectAll;
    this._getSortFunc = _getSortFunc;
    this.rowSel = rowSel;
}


/*
 * addRow
 * add a single row to the table
 *
 */
LWdatagrid.prototype.addRow = function(row) {

    if (isArray(row)) {
    var vals = new Array(row.length);
    for (var idx=0; idx<row.length; idx++)
    {
        vals[idx] = row[idx];
		
    }
    this._addRow(vals);
    }
}




/*
 * empty
 * empty the table
 *
 */
LWdatagrid.prototype.empty = function(headDelete)
{
//    for (var i=this.selectObj.length;i>=0;i--)
//    {
//        this.selectObj.options[i]=null;
//    }
//    this.selectObj.length=0;
	if(headDelete)
	{
		while(this.tableObj.header.cells.length)
    		this.tableObj.header.deleteCell(this.tableObj.header.cells.length-1);
	}
    while(this.tableObj.rows.length > 1)
    	this.tableObj.deleteRow(this.tableObj.rows.length - 1);
    
    this.data = null;
}



/*
 * getSelection
 * return an array of selected keys
 *
 */
LWdatagrid.prototype.getSelection = function()
{	
//var ret = new Array();
//        for ( i=0; i<this.selectObj.length ; i++) {
//	    if (this.selectObj.options[ i ].selected)
//	    {
//		var row = this.selectObj[i].value;
//		var rowArray = row.split('|');
//		ret.push(rowArray[0]);
//	     }
//        }
//
//return ret;
}



/*
 * init
 * init the table, to be called after body loading
 *
 */
LWdatagrid.prototype.init = function(instName, table)
{
    this.instanceName = instName;
    this.docObj = this.doc;
    this.alignArray = Array(this.columns.length);
    
    var tdObj = Array(this.columns.length);
    this.tableObj = this.docObj.createElement("TABLE");

	cellspacing = this.docObj.createAttribute('cellspacing');
	cellspacing.nodeValue = 0;
	cellpadding = this.docObj.createAttribute('cellpadding');
	cellpadding.nodeValue = 0;
	this.tableObj.setAttributeNode(cellspacing);
	this.tableObj.setAttributeNode(cellpadding);

    this.tableObj.className = 'listgrid';
    this.tableObj.style.width = "100%";
    //this.tableObj.style.height = "215px";
    this.tbodyObj = this.docObj.createElement("TBODY");
    this.tableObj.appendChild(this.tbodyObj);
    
	var fx_keyword = "Firefox/";
	var adjust_table_style = 0;
	var fx_ver_index = navigator.userAgent.indexOf(fx_keyword);
	var ver_len = 1;
	var ver_num;

	if(fx_ver_index!=-1)
	{
		fx_ver_index += fx_keyword.length;	
		ver_str = navigator.userAgent[fx_ver_index];
		if( navigator.userAgent[fx_ver_index+1] != '.' )
			ver_str += navigator.userAgent[fx_ver_index+1];

		if(parseInt(ver_str) > 4)
			adjust_table_style = 1;
						
	}
	else if(navigator.userAgent.indexOf("Chrome")!=-1)
	{
		adjust_table_style = 1;
	}

	if(adjust_table_style == 1)
    {
        table.style.height = '215px';
        table.style.overflowX = 'hidden';
        table.style.overflowY = 'auto';
    }
	else
	{
	this.tbodyObj.style.height = '215px'; //doris, work fine on firefox, cannot work on ie
	}

    var header = this.tableObj.createTHead();
    this.tableObj.header = header.insertRow(0);
    this.tableObj.header.className='head';

	if(window.ActiveXObject)
    {
// 	    header.style.position = 'absolute';
	    header.style.top = (this.tableObj.offsetTop-17)+'px';
        this.tableObj.style.width = "99%";
        table.style.height = '215px';
        table.style.overflowX = 'hidden';
        table.style.overflowY = 'auto';
     }
    for (var j=0;j<  this.columns.length;j++)
    {
	    var str = this.columns[j][0];
		this.alignArray[j] = this.columns[j][2];
		
	    tdObj[j] = this.docObj.createElement("TH");
	    if(str.toString().search(/(img)/) == -1) 
		tdObj[j].innerHTML = "&nbsp; "+ str + " &nbsp; <img src='../images/sort.gif'/>" ;
	    else //this is a image file in the table
			tdObj[j].innerHTML = "&nbsp; " + str.substring(5);
			
		
	    tdObj[j].style.textAlign = this.alignArray[j];
            tdObj[j].style.width = this.columns[j][1];		
	    tdObj[j].className = 'head';
	    tdObj[j].style.whiteSpace = 'nowrap';
		
		tdObj[j].onmouseover = function()
	    {
	        this.style.backgroundColor = '#A8B8DA';
	        this.style.cursor = 'pointer';
	        this.style.cursor = 'hand';
	    }
	
	    tdObj[j].onmouseout = function()
	    {
	        this.style.backgroundColor= '#F2F6FF';    
	    }
	    
		if(str.toString().search(/(img)/) == -1) // cannot sort image files
		{
		var idx = j+1;
		eval('tdObj[j].onclick = function(idx){'+this.instanceName+'.sortOnColumn('+idx+')};');
		}

	    this.tableObj.header.appendChild(tdObj[j]);
    }
    
    
    //table=this.docObj.getElementById(tableName);
    
    table.appendChild(this.tableObj);
//    parentObj.insertBefore(this.tableObj,this.selectObj);
//    for (var i=this.selectObj.length;i>=0;i--)
//    {
//    	this.selectObj.options[i]=null;
//    }    
}




LWdatagrid.prototype.removeRow = function(key) {
    if (isString(key))
    {
         this._removeRow(key);
    } else if (isArray(key))
    {
        for ( j=0; j<key.length ; j++)
        {
            this._removeRow(key[j])
        }
    }

}



LWdatagrid.prototype.selectAll = function()
{
    this._selectAll();
}




LWdatagrid.prototype.setColumns = function(columns) {
    this.columns = columns;
    for( i=0; i < this.columns.length; i++)
    	this.descending[i+1] = false;
}




LWdatagrid.prototype.show = function (values) {

     if  (this.data  == null)
            this.data = new Array();

    for (var idx=0; idx<values.length; idx++)
    {
        this.data[idx] = values[idx];
    }

    for (var idx=0; idx<values.length; idx++)
    {
        this.addRow(values[idx]);
    }

}


LWdatagrid.prototype.toString = function() {

//    var ret = '<select multiple="multiple" id="'+this.name+'" class="datagrid" ';
//    if (isArray(this.attributeList))
//    {
//        for (var idx=0; idx<this.attributeList.length; idx++)
//        {
//            ret += this.attributeList[idx][0] + '="' + this.attributeList[idx][1] + '" ';
//        }
//    }
//
//    ret +='>\n';
//    ret +='<option></option>\n';
//    ret +='</select>\n';
//
//    return ret;
}

LWdatagrid.prototype.addAttribute = function(act) {

    this.attributeList.push(act);
}

LWdatagrid.prototype.sortOnColumn = function(col)
{
	if(this.data.length == 0)	return;
		
	this.descending[col] = !this.descending[col];

    this.sortCol = col;
    sortFunc = this._getSortFunc();

    var values = new Array();

    for (var idx=0; idx<this.data.length; idx++)
    {
        values.push(this.data[idx][this.sortCol]);
    }
    values.sort(sortFunc);

    if (this.descending[col])
        values.reverse();
        
    var dataCache = [];

    for (var idx=0; idx<values.length; idx++)
    {
        for (var jdx=0; jdx<values.length; jdx++)
        {
            if (this.data[jdx][this.sortCol] == values[idx])
            {
            	dataCache[idx]=[];
                for(var kdx=0; kdx < this.columns.length; kdx++)
                	dataCache[idx][kdx+1] = this.data[jdx][kdx+1];
                this.data[jdx][this.sortCol] = '';
                break;
            }
        }     
    }

    this.empty();

    this.data = new Array();
    for (var idx=0; idx<dataCache.length; idx++)
    {
        this.data.push(dataCache[idx]);
    }

    this.show(this.data);
}


LWdatagrid.prototype.printView = function()
{
    var ret = '<html>\n<body onload="self.print();">\n<table>\n';
    ret += '<th>ID</th>\n';
    for (var j=0;j<  this.columns.length;j++)
    {
        ret += '<th>\n';
        ret += this.columns[j][0]+'\n';
        ret += '</th>\n';
    }
    for (var i=0; i<this.data.length; i++)
    {
    	ret += '<tr>\n';
	for (var j=0;j<  this.columns.length+1;j++)
    	{
	    ret += '<td>\n';
	    ret += this.data[i][j]+'\n';
	    ret += '</td>\n';
    	}
    	ret += '</tr>\n';
    }
    ret += '</table>\n</body>\n</html>\n';
    var printWin = window.open('#','printView','menubar=no,toolbar=no,location=no,resizable=yes,scrollbars=yes,status=no,height=300,width=300');
    printWin.document.open();
    printWin.document.write(ret);
    printWin.document.close();

}






















