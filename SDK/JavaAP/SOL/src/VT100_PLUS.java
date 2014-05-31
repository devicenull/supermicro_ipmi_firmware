import java.awt.Color;
import java.awt.Font;
import java.awt.FontFormatException;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Shape;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Vector;
import java.io.*;
import javax.swing.Timer;

class Pos {
	public int col, row;

	public Pos(int c, int r) {
		col = c;
		row = r;
	}

	public boolean equals(Object o) {
		if (o instanceof Pos) {
			Pos p = (Pos) o;
			return (p.col == col && p.row == row);
		} else {
			return false;
		}
	}
}

public class VT100_PLUS {
	protected Session parent;
	protected NVT nvt;

	protected int width, height;

	protected int maxrow, maxcol;
	protected int topmargin, buttommargin, leftmargin, rightmargin;

	protected int fontverticalgap, fonthorizontalgap, fontdescentadj;

	protected byte default_text = 0;

	protected String defFontname = "Monospaced";
	protected int defFontsize = 18, defResolution = 72;

	protected Font font;
	protected int fontwidth, fontheight, fontdescent;
	protected int resolution, fontsize;
	protected float floatFontSize;
	

	protected BufferedImage bi;
	protected Graphics g;

	protected Resource resource;

	protected int ccol, crow;

	protected int lcol, lrow;

	protected boolean linefull;

	protected byte[][] text;
	protected char[][] ctext;
	protected int[][] mbc;

	protected boolean[][] selected;

	protected boolean[][] needRepaint;
	protected List repaintList;

	protected byte[] buf;
	protected int bufpos, buflen;

	protected static byte defFg = 37;
	protected static byte defBg = 40;
	protected static byte defAttr = 0;

	protected static Color cursorColor = Color.GREEN;

	protected static final byte BOLD = 1;
	protected static final byte UNDERLINE = 8;
	protected static final byte BLINK = 16;
	protected static final byte REVERSE = 64;

	protected static final byte BEL = 7;
	protected static final byte BS = 8;
	protected static final byte TAB = 9;
	protected static final byte LF = 10;
	protected static final byte VT = 11;
	protected static final byte FF = 12;
	protected static final byte CR = 13;
	protected static final byte SO = 14;
	protected static final byte SI = 15;

	protected int blinkcount;
	protected boolean textblink, cursorblink;

	protected static final Color[] normal_colors = { new Color(0, 0, 0),
			new Color(128, 0, 0), new Color(0, 128, 0), new Color(128, 128, 0),
			new Color(0, 0, 128), new Color(128, 0, 128),
			new Color(0, 128, 128), new Color(192, 192, 192), };

	protected static final Color[] highlight_colors = {
			new Color(128, 128, 128), new Color(255, 0, 0),
			new Color(0, 255, 0), new Color(255, 255, 0), new Color(0, 0, 255),
			new Color(255, 0, 255), new Color(0, 255, 255),
			new Color(255, 255, 255), };

	protected byte[][] attributes;
	protected byte[][] fgcolors, bgcolors;

	protected byte cattribute;
	protected byte cfgcolor, cbgcolor;

	public static final int NUMERIC_KEYPAD = 1;
	public static final int APPLICATION_KEYPAD = 2;
	protected int keypadmode;

	protected int urlstate;
	protected Vector probablyurl;
	protected boolean[][] isurl;

	protected Timer ti;

	protected Convertor conv;

	public void repaintAll() {
		for (int i = 1; i <= maxrow; i++) {
			for (int j = 1; j <= maxcol; j++) {
				setRepaint(j, i);
			}
		}
	}

	public void updateImage(BufferedImage b) {
		bi = b;
		g = bi.createGraphics();
	}

	public void updateSize(int w, int h) {
		width = w;
		height = h;
	}

	public void updateFont() {
		FontMetrics fm;
		float fh, fw;

		fonthorizontalgap = 1;
		fontverticalgap = 1;
		fontdescentadj = resource.getIntValue(Resource.FONT_DESCENT_ADJUST);

		fontsize = resource.getIntValue(Resource.FONT_SIZE);
		if (fontsize == 0) {
			fw = (float)960 / maxcol - fonthorizontalgap;
			fh = (float)552 / maxrow - fontverticalgap;
			if (fh > 2 * fw) {
				fh = 2 * fw;
			}
			floatFontSize = fh;
			fontsize = (int)fh;
		}

		font = new Font(defFontname, Font.PLAIN, fontsize);
		font = font.deriveFont(floatFontSize);
		if (resource.getBooleanValue(Resource.FONT_USE_CUSTOM_FONT)) {
			try {
				File f = new File(resource.getStringValue(Resource.FONT_TTF_PATH));
				Font basefont = Font.createFont(Font.TRUETYPE_FONT, new FileInputStream(f));
				font = basefont.deriveFont(Font.PLAIN, fontsize);
			} catch (FontFormatException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		g.setFont(font);

		fm = g.getFontMetrics();

		fontheight = fontsize;
		fontwidth = fontsize / 2;
		fontdescent = (int) (1.0 * fm.getDescent() / fm.getHeight() * fontsize);

		fontheight += fontverticalgap;
		fontwidth += fonthorizontalgap;
		fontdescent += fontdescentadj;
	}

	public void updateScreen() {
		repaintAll();
	}

	protected char bytesToChar(byte[] b, int from, int limit) {
		char ch = conv.big5BytesToUCS2(b[from], b[from + 1]);
		return ch; 
	}

	protected byte getNextByte() {
		if (bufpos == buflen) {
			try {
				buflen = nvt.readBytes(buf);
			} catch (IOException e) {
				parent.close(true);
			}
			bufpos = 0;
		}

		return buf[bufpos++];
	}

	public boolean isBufferEmpty() {
		return (bufpos == buflen);
	}

	protected void setRepaint(int col, int row) {
		if (row >= 1 && row <= maxrow && col >= 1 && col <= maxcol
				&& !needRepaint[row - 1][col - 1]) {
			needRepaint[row - 1][col - 1] = true;
			repaintList.add(new Pos(col, row));
		}
	}

	protected Pos getRepaint() {
		Pos p = (Pos) repaintList.get(0);
		needRepaint[p.row - 1][p.col - 1] = false;
		repaintList.remove(0);

		return p;
	}

	protected boolean isRepaintListEmpty() {
		return repaintList.isEmpty();
	}

	public void resetSelected() {
		for (int i = 1; i <= maxrow; i++) {
			for (int j = 1; j <= maxcol; j++) {
				selected[i - 1][j - 1] = false;
				setRepaint(j, i);
			}
		}
	}

	public void setSelected(int x1, int y1, int x2, int y2) {
		int i, j;
		int r1, c1, r2, c2, tmp;

		c1 = x1 / fontwidth + 1;
		r1 = y1 / fontheight + 1;

		c2 = x2 / fontwidth + 1;
		r2 = y2 / fontheight + 1;

		if (r1 > r2) {
			tmp = r1;
			r1 = r2;
			r2 = tmp;

			tmp = c1;
			c1 = c2;
			c2 = tmp;
		} else if (r1 == r2) {
			if (c1 > c2) {
				tmp = c1;
				c1 = c2;
				c2 = tmp;
			}
		}

		for (i = 1; i <= maxrow; i++) {
			for (j = 1; j <= maxcol; j++) {

				if (i > r1 && i < r2) {
					selected[i - 1][j - 1] = true;
				} else if (i == r1 && i == r2) {
					selected[i - 1][j - 1] = (j >= c1 && j <= c2);
				} else if (i == r1) {
					selected[i - 1][j - 1] = (j >= c1);
				} else if (i == r2) {
					selected[i - 1][j - 1] = (j <= c2);
				} else {
					selected[i - 1][j - 1] = false;
				}

				setRepaint(j, i);
			}
		}
	}

	public void selectConsequtive(int x, int y) {
		int c, r;
		int i, beginx, endx;

		c = x / fontwidth + 1;
		r = y / fontheight + 1;

		if (c <= 0 || c > maxcol || r <= 0 || r > maxrow) {
			return;
		}

		for (beginx = c; beginx > 0 && text[r - 1][beginx - 1] != default_text
				&& text[r - 1][beginx - 1] != ' '; beginx--)
			;
		for (endx = c; endx < maxcol && text[r - 1][endx - 1] != default_text
				&& text[r - 1][endx - 1] != ' '; endx++)
			;

		resetSelected();
		for (i = beginx + 1; i < endx; i++) {
			selected[r - 1][i - 1] = true;
			setRepaint(i, r);
		}
	}

	public void selectEntireLine(int x, int y) {
		int i, r;

		r = y / fontheight + 1;

		resetSelected();
		for (i = 1; i < maxcol; i++) {
			selected[r - 1][i - 1] = true;
			setRepaint(i, r);
		}
	}

	public String getSelectedText() {
		int i, j;
		boolean needNewLine = false, newLineAdded, currentLineHasSelected;
		StringBuffer sb = new StringBuffer();

		for (i = 1; i <= maxrow; i++) {
			currentLineHasSelected = newLineAdded = false;
			for (j = 1; j <= maxcol; j++) {
				if (selected[i - 1][j - 1]) {

					if (needNewLine && (newLineAdded == false)) {
						sb.append("\n");
						newLineAdded = true;
					}

					if (text[i - 1][j - 1] != default_text) {
						if (isWideChar(text[i - 1][j - 1]) && j < maxcol - 1) {
							sb.append(bytesToChar(text[i - 1], j - 1, 2));
							j++;
						} else {
							sb.append((char) text[i - 1][j - 1]);
						}
					}
					currentLineHasSelected = true;
				}
			}
			needNewLine = currentLineHasSelected;
		}

		return new String(sb);
	}

	public void pasteText(String str) {
		char[] ca;
		boolean autobreak;
		int breakcount;

		if (str == null) {
			return;
		}

		autobreak = resource.getBooleanValue(Resource.AUTO_LINE_BREAK);
		breakcount = resource.getIntValue(Resource.AUTO_LINE_BREAK_LENGTH);

		if (autobreak) {
			str = TextUtils.fmt(str, breakcount);
		}

		ca = str.toCharArray();

		for (int i = 0; i < ca.length; i++) {
			if (ca[i] == 0x0a) {
				ca[i] = 0x0d;
			}
		}

		parent.writeChars(ca, 0, ca.length);
	}

	protected void reset(int col, int row) {
		resetText(col, row);
		resetAttr(col, row);
		setRepaint(col, row);
	}

	protected void resetAttr(int col, int row) {
		fgcolors[row - 1][col - 1] = defFg;
		bgcolors[row - 1][col - 1] = defBg;
		attributes[row - 1][col - 1] = defAttr;

		isurl[row - 1][col - 1] = false;
	}

	protected void resetText(int col, int row) {
		text[row - 1][col - 1] = default_text;
		ctext[row - 1][col - 1] = (char) default_text;
		mbc[row - 1][col - 1] = 0;
	}

	protected void copy(int dstcol, int dstrow, int srccol, int srcrow) {
		text[dstrow - 1][dstcol - 1] = text[srcrow - 1][srccol - 1];
		ctext[dstrow - 1][dstcol - 1] = ctext[srcrow - 1][srccol - 1];
		mbc[dstrow - 1][dstcol - 1] = mbc[srcrow - 1][srccol - 1];

		fgcolors[dstrow - 1][dstcol - 1] = fgcolors[srcrow - 1][srccol - 1];
		bgcolors[dstrow - 1][dstcol - 1] = bgcolors[srcrow - 1][srccol - 1];
		attributes[dstrow - 1][dstcol - 1] = attributes[srcrow - 1][srccol - 1];

		isurl[dstrow - 1][dstcol - 1] = isurl[srcrow - 1][srccol - 1];

		setRepaint(dstcol, dstrow);
	}

	protected void eraseline(int row, int mode) {
		int i, begin, end;

		switch (mode) {
		case 0:
			begin = ccol;
			end = rightmargin;
			break;
		case 1:
			begin = leftmargin;
			end = ccol;
			break;
		case 2:
			begin = leftmargin;
			end = rightmargin;
			break;
		default:
			begin = leftmargin;
			end = rightmargin;
			break;
		}

		for (i = begin; i <= end; i++) {
			reset(i, row);
		}
	}

	protected void erasescreen(int mode) {
		int i, begin, end;

		switch (mode) {
		case 0:
			eraseline(crow, mode);
			begin = crow + 1;
			end = maxrow;
			break;
		case 1:
			eraseline(crow, mode);
			begin = 1;
			end = crow - 1;
			break;
		case 2:
			begin = 1;
			end = maxrow;
			break;
		default:
			begin = 1;
			end = maxrow;
			break;
		}

		for (i = begin; i <= end; i++) {
			eraseline(i, 2);
		}
	}

	protected void insertline(int r, int n) {

		for (int i = buttommargin; i >= r; i--) {
			for (int j = leftmargin; j <= rightmargin; j++) {
				if (i >= r + n) {
					copy(j, i, j, i - n);
				} else {
					reset(j, i);
				}
			}
		}
	}

	protected void reverseindex() {

		if (crow == topmargin) {
			insertline(crow, 1);
		} else {
			crow--;
		}
	}

	protected void setmargin(int top, int buttom) {
		topmargin = top;
		buttommargin = buttom;
	}

	protected void scrollpage(int line) {
		Pos p;

		for (int i = topmargin; i <= buttommargin; i++) {
			for (int j = leftmargin; j <= rightmargin; j++) {

				if (i <= buttommargin - line) {
					copy(j, i, j, i + line);
				} else {
					reset(j, i);
				}

			}
		}

		for (int i = 0; i < probablyurl.size(); i++) {
			p = (Pos) probablyurl.elementAt(i);
			probablyurl.setElementAt(new Pos(p.col, p.row - line), i);
		}
	}

	protected boolean isWideChar(byte a) {
		if (a < 0)
			return true;
		return false;
	}

	protected void bell() {
		parent.bell();
	}

	public void paint(Graphics gg) {
		drawText();

		gg.drawImage(bi, 0, 0, null);
	}

	protected void initResource() {
		maxcol = 100;
		maxrow = 31;
	}

	protected void initValue() {
		cfgcolor = defFg;
		cbgcolor = defBg;
		cattribute = defAttr;

		urlstate = 0;
		probablyurl = new Vector();

		blinkcount = 0;
		textblink = true;
		cursorblink = true;

		ccol = 1;
		crow = 1;
		lcol = 1;
		lrow = 1;
		linefull = false;

		topmargin = 1;
		buttommargin = maxrow;
		leftmargin = 1;
		rightmargin = maxcol;

		keypadmode = NUMERIC_KEYPAD;
	}

	protected void initArray() {
		text = new byte[maxrow][maxcol];
		ctext = new char[maxrow][maxcol];
		mbc = new int[maxrow][maxcol];

		selected = new boolean[maxrow][maxcol];
		needRepaint = new boolean[maxrow][maxcol];

		fgcolors = new byte[maxrow][maxcol];
		bgcolors = new byte[maxrow][maxcol];
		attributes = new byte[maxrow][maxcol];

		isurl = new boolean[maxrow][maxcol];

		repaintList = Collections.synchronizedList(new LinkedList());

		for (int i = 0; i < maxrow; i++) {
			for (int j = 0; j < maxcol; j++) {
				needRepaint[i][j] = false;

				fgcolors[i][j] = defFg;
				bgcolors[i][j] = defBg;
				attributes[i][j] = defAttr;

				isurl[i][j] = false;

				reset(j + 1, i + 1);
			}
		}
	}

	protected void initOthers() {
		ti = new Timer(250, new RepaintTask());
		ti.start();
	}

	public boolean coverURL(int x, int y) {
		int c = x / fontwidth, r = y / fontheight;

		if (r < maxrow && c < maxcol) {
			return isurl[r][c];
		}

		return false;
	}

	public String getURL(int x, int y) {
		StringBuffer sb = new StringBuffer();
		int i;
		int c = x / fontwidth, r = y / fontheight;

		if (r >= maxrow && c >= maxcol) {
			return new String();
		}

		for (i = c; i >= 0 && isurl[r][i]; i--)
			;
		for (i++; i < maxcol && isurl[r][i]; i++) {
			sb.append((char) text[r][i]);
		}

		return new String(sb);
	}

	protected void setColor(int c) {
		if (c == 0) {
			cfgcolor = defFg;
			cbgcolor = defBg;
			cattribute = defAttr;
		} else if (c == 1) {
			cattribute |= BOLD;
		} else if (c == 4) {
			cattribute |= UNDERLINE;
		} else if (c == 5) {
			cattribute |= BLINK;
		} else if (c == 7) {
			cattribute |= REVERSE;
		} else if (30 <= c && c <= 37) {
			cfgcolor = (byte) c;
		} else if (40 <= c && c <= 47) {
			cbgcolor = (byte) c;
		}
	}

	public int getKeypadMode() {
		return keypadmode;
	}

	public Color getFgColor(int col, int row) {
		Color c;

		if ((attributes[row - 1][col - 1] & BOLD) != 0) {
			c = highlight_colors[fgcolors[row - 1][col - 1] - 30];
		} else {
			c = normal_colors[fgcolors[row - 1][col - 1] - 30];
		}

		if ((attributes[row - 1][col - 1] & REVERSE) != 0) {
			c = reverseColor(c);
		}

		return c;
	}

	public Color getBgColor(int col, int row) {
		Color c = normal_colors[bgcolors[row - 1][col - 1] - 40];

		if ((attributes[row - 1][col - 1] & REVERSE) != 0) {
			c = reverseColor(c);
		}

		return c;
	}

	protected static Color reverseColor(Color c) {
		int r = c.getRed(), g = c.getGreen(), b = c.getBlue();

		r = 255 - r;
		g = 255 - g;
		b = 255 - b;

		return new Color(r, g, b);
	}

	protected void parse_csi() {
		int argc;
		int arg;
		int[] argv = new int[256];
		byte b;

		arg = -1;
		argc = 0;
		while (true) {
			b = getNextByte();

			if ('0' <= b && b <= '9') {
				if (arg == -1) {
					arg = 0;
				} else {
					arg *= 10;
				}
				
				arg += b - '0';
			} else if (b == ';') {
				argv[argc] = arg;
				argc++;
				arg = 0;
			} else if (b == '?' || b == '!' || b == '"' || b == '\'') {
				;
			} else {
				argv[argc] = arg;
				argc++;
				break;
			}
		}

		switch (b) {
		case 'h': 
			System.out.print("Set mode(Not yet support): ");
			for (int i = 0; i < argc; i++) {
				System.out.print(argv[i] + " ");
			}
			System.out.println();
			break;
		case 'm':
			for (int i = 0; i < argc; i++) {
				if (argv[i] == -1) {
					argv[i] = 0;
				}
				setColor(argv[i]);
			}

			break;
		case 'l': 
			System.out.println("Reset mode(Not yet support)");
			break;
		case 'r': 
			setmargin(argv[0], argv[1]);

			break;
		case 'A':
			if (argv[0] == -1) {
				argv[0] = 1;
			}
			crow = Math.max(crow - argv[0], topmargin);

			break;
		case 'B':
			if (argv[0] == -1) {
				argv[0] = 1;
			}
			crow = Math.min(crow + argv[0], buttommargin);

			break;
		case 'C':
			if (argv[0] == -1) {
				argv[0] = 1;
			}
			ccol = Math.min(ccol + argv[0], rightmargin);

			break;
		case 'D':
			if (argv[0] == -1) {
				argv[0] = 1;
			}
			ccol = Math.max(ccol - argv[0], leftmargin);

			break;
		case 'H':

			if (argv[0] < 1) {
				argv[0] = 1;
			}

			if (argv[1] < 1) {
				argv[1] = 1;
			}

			crow = Math.min(Math.max(argv[0], topmargin), buttommargin);
			ccol = Math.min(Math.max(argv[1], leftmargin), rightmargin);

			break;
		case 'J':
			if (argv[0] == -1) {
				argv[0] = 0;
			}
			erasescreen(argv[0]);

			break;
		case 'K':
			if (argv[0] == -1) {
				argv[0] = 0;
			}
			eraseline(crow, argv[0]);

			break;
		case 'L':
			if (argv[0] == -1) {
				argv[0] = 1;
			}
			insertline(crow, argv[0]);

			break;
		default:
			System.out.println("Unknown control sequence: " + b);
			break;
		}
	}

	protected void parse_text_parameter() {
		byte b;
		byte[] text = new byte[512];
		int f, count;

		f = 0;
		b = getNextByte();
		while (b != ';') {
			f *= 10;
			f += b - '0';
			b = getNextByte();
		}

		count = 0;
		b = getNextByte();
		while (b != BEL) {
			text[count++] = b;
			b = getNextByte();
		}

		switch (f) {
		case 0:
			parent.setIconName(new String(text, 0, count));
			parent.setWindowTitle(new String(text, 0, count));
			break;
		case 1:
			parent.setIconName(new String(text, 0, count));
			break;
		case 2:
			parent.setWindowTitle(new String(text, 0, count));
			break;
		default:
			System.out.println("Set text parameters(not fully support)");
			break;
		}
	}

	protected void parse_scs(byte a) {
		byte b;

		System.out.print("SCS(Not yet support) ");

		b = getNextByte();

		System.out.println(a + " " + b);
	}

	protected void parse_esc() {
		byte b;

		b = getNextByte();

		switch (b) {
		case '(': 
		case ')': 
			parse_scs(b);
			break;
		case '=': 
			keypadmode = APPLICATION_KEYPAD;

			break;
		case '>': 
			keypadmode = NUMERIC_KEYPAD;

			break;
		case 'M': 
			reverseindex();

			break;
		case '[': 
			parse_csi();
			break;
		case ']':
			parse_text_parameter();
			break;
		default:
			System.out.println("Unknown control charecter: " + b);
			break;
		}
	}

	protected void parse() {
		byte b;
		boolean control;

		b = getNextByte();

		lcol = ccol;
		lrow = crow;
		

		control = true;

		switch (b) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			break;
		case 7: 
			bell();

			if (!parent.isTabForeground()) {
				parent.setIcon(S_O_L.ICON_ALERT);
			}

			break;
		case 8: 
			if (linefull) {
				linefull = false;
			} else if (ccol > leftmargin) {
				ccol--;
			} else if (ccol == leftmargin && crow > topmargin) {
				ccol = rightmargin;
				crow--;
			}
			break;
		case 9: 
			ccol = ((ccol - 1) / 8 + 1) * 8 + 1;
			break;
		case 10: 
			crow++;
			if (crow > buttommargin) {
				scrollpage(1);
				crow--;
			}
			break;
		case 11:
			break;
		case 12:
			break;
		case 13: 
			ccol = leftmargin;
			break;
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
			break;
		case 27: 
			parse_esc();
			break;
		case 28:
		case 29:
		case 30:
		case 31:
			break;
		default:
			control = false;
			break;
		}

		if (ccol != rightmargin) {
			linefull = false;
		}
		
		if(crow > buttommargin || ccol > rightmargin )
			return;

		if (control == false) {

			if (linefull) {
				linefull = false;
				ccol = leftmargin;
				crow++;
				if (crow > buttommargin) {
					scrollpage(1);
					crow--;
				}
				setRepaint(leftmargin, crow);
			}


			try {
				text[crow - 1][ccol - 1] = b;

				if (UtilUnicode.isUnicode(b)) {
					text[crow - 1][ccol - 1] = 65;
				}

				isurl[crow - 1][ccol - 1] = false;
			} catch(Exception e) {
				e.printStackTrace();
			}

			checkURL((char) b);

			
			if (UtilUnicode.isUnicode(b)) {
				int len = UtilUnicode.bytesToEncode(b);
				
				if (len != 0) {
					byte[] buffer = new byte[len];
					
					buffer[0] = b;
					
					for(int i = 1; i < len; i++) {
						byte data = getNextByte();
						buffer[i] = data;
					}
					try {
						ctext[crow - 1][ccol - 1] = (new String(buffer, "UTF-8")).charAt(0);
					} catch (UnsupportedEncodingException e) {
						e.printStackTrace();
					}
				} else {
					ctext[crow - 1][ccol - 1] = (char) b;
				}
			} else {
				ctext[crow - 1][ccol - 1] = (char) b;
			}
			
			if (UtilUnicode.isUnicode(b) && UtilUnicode.isHan(ctext[crow - 1][ccol - 1]))
			{
				if(ccol <= rightmargin){
					ctext[crow - 1][ccol ] = 0;
					mbc[crow - 1][ccol - 1] = 1;
					mbc[crow - 1][ccol ] = 2;
				}
				ccol++;
				if (ccol > rightmargin) {
					linefull = true;
					ccol--;
				}
			}
			else
				mbc[crow - 1][ccol - 1] = 1;
			
			if(S_O_L.terminalType == 1 && text[crow - 1][ccol - 1] < 0)
			{
				ctext[crow - 1][ccol - 1] = ANSItoUTF8.translate(text[crow - 1][ccol - 1]);
			}
			fgcolors[crow - 1][ccol - 1] = cfgcolor;
			bgcolors[crow - 1][ccol - 1] = cbgcolor;
			attributes[crow - 1][ccol - 1] = cattribute;

			ccol++;
			if (ccol > rightmargin) {
				linefull = true;
				ccol--;
			}

		} else { 
			checkURL((char) b);
		}

		setRepaint(lcol, lrow);
		if (lcol != ccol || lrow != crow) {
			setRepaint(ccol, crow);
		}
	}

	protected void checkURL(char c) {
		String W = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ;/?:@&=+$,-_.!~*'%#";
		Iterator iter;

		switch (urlstate) {
		case 0:
			probablyurl.removeAllElements();
			if (c == 'h') {
				urlstate = 1;
				probablyurl.addElement(new Pos(ccol, crow));
			}
			break;
		case 1:
			if (c == 't') {
				urlstate = 2;
				probablyurl.addElement(new Pos(ccol, crow));
			} else {
				urlstate = 0;
			}
			break;
		case 2:
			if (c == 't') {
				urlstate = 3;
				probablyurl.addElement(new Pos(ccol, crow));
			} else {
				urlstate = 0;
			}
			break;
		case 3:
			if (c == 'p') {
				urlstate = 4;
				probablyurl.addElement(new Pos(ccol, crow));
			} else {
				urlstate = 0;
			}
			break;
		case 4:
			if (c == ':') {
				urlstate = 6;
				probablyurl.addElement(new Pos(ccol, crow));
			} else if (c == 's') {
				urlstate = 5;
				probablyurl.addElement(new Pos(ccol, crow));
			} else {
				urlstate = 0;
			}
			break;
		case 5:
			if (c == ':') {
				urlstate = 6;
				probablyurl.addElement(new Pos(ccol, crow));
			} else {
				urlstate = 0;
			}
			break;
		case 6:
			if (c == '/') {
				urlstate = 7;
				probablyurl.addElement(new Pos(ccol, crow));
			} else {
				urlstate = 0;
			}
			break;
		case 7:
			if (c == '/') {
				urlstate = 8;
				probablyurl.addElement(new Pos(ccol, crow));
			} else {
				urlstate = 0;
			}
			break;
		case 8:
			if (W.indexOf(c) != -1) {
				urlstate = 9;
				probablyurl.addElement(new Pos(ccol, crow));
			} else {
				urlstate = 0;
			}
			break;
		case 9:
			if (W.indexOf(c) == -1) {
				iter = probablyurl.iterator();
				while (iter.hasNext()) {
					Pos p = (Pos) iter.next();
					isurl[p.row - 1][p.col - 1] = true;
					setRepaint(p.col, p.row);
				}
				urlstate = 0;
			} else {
				probablyurl.addElement(new Pos(ccol, crow));
			}
			break;
		default:
			urlstate = 0;
		}
	}

	public String makePasteText(Vector a, Vector b, Vector fg, Vector bg) {
		int i, j, c;

		byte cattr, tmpattr;
		byte cfg, cbg, tmpfg, tmpbg;
		byte mask;

		boolean needReset, needControl, isFirst;
		StringBuffer sb = new StringBuffer();

		char[] buf = new char[32];

		buf[0] = 21;
		buf[1] = '[';
		buf[2] = '0';
		buf[3] = 'm';
		sb.append(buf, 0, 4);

		cattr = 0;
		cfg = defFg;
		cbg = defBg;

		for (i = 0; i < a.size(); i++) {

			tmpattr = ((Byte) a.elementAt(i)).byteValue();
			tmpfg = ((Byte) fg.elementAt(i)).byteValue();
			tmpbg = ((Byte) bg.elementAt(i)).byteValue();

			for (mask = 1, j = 0, needReset = needControl = false; j < 7; j++) {
				if (((cattr & (mask << j)) != 0)
						&& ((tmpattr & (mask << j)) == 0)) {
					needReset = true;
				}

				if ((cattr & (mask << j)) != (tmpattr & (mask << j))) {
					needControl = true;
				}

				if (cfg != tmpfg || cbg != tmpbg) {
					needControl = true;
				}
			}

			if (!needControl) {
				sb.append((char) ((Byte) b.elementAt(i)).byteValue());
				continue;
			}


			buf[0] = 21;
			buf[1] = '[';

			if (needReset) {
				buf[2] = '0';

				for (mask = 1, j = 0, c = 3; j < 7; j++) {
					if ((tmpattr & (mask << j)) != 0) {
						buf[c++] = ';';
						buf[c++] = (char) ('1' + j);
					}
				}

				if (tmpfg != defFg) {
					buf[c++] = ';';
					buf[c++] = (char) ((tmpfg / 10) + '0');
					buf[c++] = (char) ((tmpfg % 10) + '0');
				}

				if (tmpbg != defBg) {
					buf[c++] = ';';
					buf[c++] = (char) ((tmpbg / 10) + '0');
					buf[c++] = (char) ((tmpbg % 10) + '0');
				}

			} else {

				isFirst = true;
				for (mask = 1, j = 0, c = 2; j < 7; j++) {

					if ((tmpattr & (mask << j)) != 0
							&& (cattr & (mask << j)) == 0) {
						if (isFirst) {
							isFirst = false;
						} else {
							buf[c++] = ';';
						}
						buf[c++] = (char) ('1' + j);
					}
				}

				if (cfg != tmpfg) {
					if (isFirst) {
						isFirst = false;
					} else {
						buf[c++] = ';';
					}
					buf[c++] = (char) ((tmpfg / 10) + '0');
					buf[c++] = (char) ((tmpfg % 10) + '0');
				}

				if (cbg != tmpbg) {
					if (isFirst) {
						isFirst = false;
					} else {
						buf[c++] = ';';
					}
					buf[c++] = (char) ((tmpbg / 10) + '0');
					buf[c++] = (char) ((tmpbg % 10) + '0');
				}
			}

			buf[c++] = 'm';
			buf[c++] = (char) ((Byte) b.elementAt(i)).byteValue();
			sb.append(buf, 0, c);

			cattr = tmpattr;
			cfg = tmpfg;
			cbg = tmpbg;
		}
		return new String(sb);
	}

	public String getSelectedColorText() {
		int i, j;
		Vector a = new Vector(); 
		Vector b = new Vector(); 
		Vector fg = new Vector(); 
		Vector bg = new Vector(); 
		boolean needNewLine;

		for (i = 1; i <= maxrow; i++) {
			needNewLine = false;
			for (j = 1; j <= maxcol; j++) {
				if (selected[i - 1][j - 1]) {
					if (text[i - 1][j - 1] != default_text) {
						a.addElement(new Byte(attributes[i - 1][j - 1]));
						b.addElement(new Byte(text[i - 1][j - 1]));
						fg.addElement(new Byte(fgcolors[i - 1][j - 1]));
						bg.addElement(new Byte(bgcolors[i - 1][j - 1]));
					}
					needNewLine = true;
				}
			}
			if (needNewLine) {
				a.addElement(new Byte((byte) 0));
				b.addElement(new Byte((byte) 0x0d));
				fg.addElement(new Byte(defFg));
				bg.addElement(new Byte(defBg));
			}
		}

		return makePasteText(a, b, fg, bg);
	}

	public void pasteColorText(String str) {
		byte[] tmp = new byte[str.length()];

		for (int i = 0; i < str.length(); i++) {
			tmp[i] = (byte) str.charAt(i);
		}

		parent.writeBytes(tmp, 0, tmp.length);
	}

	protected void drawText() {
		Pos p;
		int i, j;
		int w, h;

		while (!isRepaintListEmpty()) {

			p = getRepaint();
			i = p.row - 1;
			j = p.col - 1;

			h = i * fontheight;
			w = j * fontwidth;

			if (selected[i][j]) {
				g.setColor(reverseColor(getBgColor(p.col, p.row)));
			} else {
				g.setColor(getBgColor(p.col, p.row));
			}

			if (crow == p.row && ccol == p.col && cursorblink) {
				g.setColor(cursorColor);
			}

			g.fillRect(w, h, fontwidth, fontheight);

			if (((attributes[i][j] & UNDERLINE) != 0) || isurl[i][j]) {
				if (isurl[i][j]) {
					g.setColor(Color.ORANGE);
				} else {
					g.setColor(getFgColor(p.col, p.row));
				}
				g.drawLine(w, h + fontheight - 1, w + fontwidth - 1, h
						+ fontheight - 1);
			}

			if (text[i][j] == default_text) {
				continue;
			}

			if (selected[i][j]) {
				g.setColor(reverseColor(getFgColor(p.col, p.row)));
			} else {
				g.setColor(getFgColor(p.col, p.row));
			}

			if (((attributes[i][j] & BLINK) == 0) || textblink) {
				Shape oldclip = g.getClip();
				g.clipRect(w, h, fontwidth*2, fontheight);
				try {
					g.drawChars(
							ctext[i],
							j - (mbc[i][j] - 1),
							1,
							w - fontwidth * (mbc[i][j] -1),
							h + fontheight - fontdescent
						);
				} catch(ArrayIndexOutOfBoundsException e) {
					e.printStackTrace();
				}
				
				g.setClip(oldclip);
			}

		}
	}

	public void close() {
		ti.stop();
	}

	class RepaintTask implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			blinkcount++;

			if (blinkcount % 2 == 0) {
				cursorblink = !cursorblink;
				setRepaint(ccol, crow);
				parent.repaint();
			}

			if (blinkcount % 3 == 0) {
				textblink = !textblink;
				for (int i = 1; i <= maxcol; i++) {
					for (int j = 1; j <= maxrow; j++) {
						if ((attributes[j - 1][i - 1] & BLINK) != 0) {
							setRepaint(i, j);
						}
					}
				}
				parent.repaint();
			}
		}
	}

	public VT100_PLUS(int w, int h, Network n, Session p, BufferedImage b,
			Resource r, Convertor c) {

		nvt = new NVT(n);

		parent = p;
		resource = r;
		conv = c;

		buf = new byte[4096];
		bufpos = buflen = 0;

		initResource();

		initArray();
		initValue();
		initOthers();

		updateImage(b);
		updateSize(w, h);
		updateFont();
		updateScreen();
	}
	
	static class ANSItoUTF8
	{
		public static char translate(byte input_ansi)
		{
			char output_utf8 = 0;
			char []map = new char[]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									9617,9618,9619,9475,9508,0,0,0,0,0,0,0,0,0,0,9488,
									9492,9524,9516,9500,9472,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,9496,9484,9608,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
			
			if(input_ansi < 0)
			{
				output_utf8 = map[input_ansi + 128];
			}
			
			return output_utf8; 
		}
	}
}
