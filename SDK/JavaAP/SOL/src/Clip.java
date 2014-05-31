import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.ClipboardOwner;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.Toolkit;
import java.io.IOException;

public class Clip implements ClipboardOwner {

	public void lostOwnership(Clipboard clipboard, Transferable contents) {
	}

	public void setContent(String str) {
		StringSelection ss = new StringSelection(str);
		Clipboard clip = Toolkit.getDefaultToolkit().getSystemClipboard();
		clip.setContents(ss, this);
	}

	public String getContent() {
		String result = null;
		Clipboard clip = Toolkit.getDefaultToolkit().getSystemClipboard();
		Transferable contents = clip.getContents(null);
		boolean hasTransferableText;

		if (contents == null) {
			hasTransferableText = false;
		} else if (!contents.isDataFlavorSupported(DataFlavor.stringFlavor)) {
			hasTransferableText = false;
		} else {
			hasTransferableText = true;
		}

		if (hasTransferableText) {
			try {
				result = (String) contents.getTransferData(DataFlavor.stringFlavor);
			} catch (UnsupportedFlavorException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		return result;
	}
}
