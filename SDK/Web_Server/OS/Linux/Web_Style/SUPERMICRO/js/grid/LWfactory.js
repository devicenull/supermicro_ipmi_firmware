/* 
 * LWCfactory.js 
 * This the widgets factory
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

var LWC = new LWCfactory();

/*
 * LWCfactory
 * factory class - this allow to load once the js files, e.g. into the top frames,
 * next we can easily obtain an LWComponents instance 
 *
 * (example from within a frameset child for a datagrid component)
 *
 * var myDatagrid = top.LWC.getElement('datagrid',document);
 *
 */
function LWCfactory()
{
    LWCfactory.prototype.getElement = function(elementName,doc) 
    {
        var obj = null;
        if (doc == null)
            doc = document;
        eval ('obj = new LW'+elementName+'(doc);');
        return obj;
    }
}
