/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  PSerial - class for serial port goodness
  Part of the Processing project - http://processing.org

  Copyright (c) 2004 Ben Fry & Casey Reas

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

package processing.app;
//import processing.core.*;

import static processing.app.I18n._;
import gnu.io.CommPortIdentifier;
import gnu.io.CommPortOwnershipListener;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.List;

import processing.app.debug.ArmCompiler;
import processing.app.debug.MessageConsumer;




public class Serial implements SerialPortEventListener, CommPortOwnershipListener {

  //PApplet parent;

  // properties can be passed in for default values
  // otherwise defaults to 9600 N81

  // these could be made static, which might be a solution
  // for the classloading problem.. because if code ran again,
  // the static class would have an object that could be closed

  SerialPort port;

  int rate;
  int parity;
  int databits;
  int stopbits;
  boolean monitor = false;
  Editor.DefaultExportHandler proxyEditorForThreadControl = null;
  // read buffer and streams 

  InputStream input;
  OutputStream output;

  byte buffer[] = new byte[32768];
  int bufferIndex;
  int bufferLast;
  
  MessageConsumer consumer;

//[ROBOTIS][CHANGE][START] to support pandora cm-900 board
 
  public SharedObjectForThreadControl robotisSharedObject = null;
  boolean pandoraBoard = true;
  static final String downloadReady = "Ready.." ;
  static final String downloadSuccess = "Success.." ;
  static final String downloadFail = "Fail.." ;
//[ROBOTIS][CHANGE][END] to support pandora cm-900 board
  
  /*public void setProxyEditorForThreadControl(Editor proxy){
	  proxyEditorForThreadControl = proxy;
  }*/
  public Serial(boolean monitor) throws SerialException {
    this(Preferences.get("serial.port"),
      Preferences.getInteger("serial.debug_rate"),
      Preferences.get("serial.parity").charAt(0),
      Preferences.getInteger("serial.databits"),
      new Float(Preferences.get("serial.stopbits")).floatValue());
    this.monitor = monitor;
  }
    
  public Serial() throws SerialException {
    this(Preferences.get("serial.port"),
      Preferences.getInteger("serial.debug_rate"),
      Preferences.get("serial.parity").charAt(0),
      Preferences.getInteger("serial.databits"), 
      new Float(Preferences.get("serial.stopbits")).floatValue());
  }

  public Serial(int irate) throws SerialException {	  
	    this(Preferences.get("serial.port"), irate,
	      Preferences.get("serial.parity").charAt(0),
	      Preferences.getInteger("serial.databits"), 
	      new Float(Preferences.get("serial.stopbits")).floatValue());
	   
	  }
  
  public Serial(int irate, SharedObjectForThreadControl so) throws SerialException {	  
    this(Preferences.get("serial.port"), irate,
      Preferences.get("serial.parity").charAt(0),
      Preferences.getInteger("serial.databits"), 
      new Float(Preferences.get("serial.stopbits")).floatValue());
    robotisSharedObject = so;
  }

  public Serial(String iname, int irate) throws SerialException {
    this(iname, irate, Preferences.get("serial.parity").charAt(0),
    Preferences.getInteger("serial.databits"), 
    new Float(Preferences.get("serial.stopbits")).floatValue());
  }

  public Serial(String iname) throws SerialException {
    this(iname, Preferences.getInteger("serial.debug_rate"),
      Preferences.get("serial.parity").charAt(0),
      Preferences.getInteger("serial.databits"),
      new Float(Preferences.get("serial.stopbits")).floatValue());
  }

  public static boolean checkPort(){
	  SerialPort testPort = null;
	  String iname = Preferences.get("serial.port");
	  if(iname == null)
		  return false;
	  Enumeration portList = CommPortIdentifier.getPortIdentifiers();
      while (portList.hasMoreElements()) {
        CommPortIdentifier portId =
          (CommPortIdentifier) portList.nextElement();

        if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
         // System.out.println("found " + portId.getName());
          if (portId.getName().equals(iname)) {
        	  try {
				 testPort = (SerialPort)portId.open("CM9SerialDownloader", 2000);
			} catch (PortInUseException e) {
				// TODO Auto-generated catch block
				//e.printStackTrace();
				System.out.println("[Serial]Port is already used in "+e.currentOwner);
				//testPort.close();
				return false;
				
			}
        	 // System.out.println("[SERIAL]port is ok!");
        	  testPort.close();
        	  return true;
          }
        }
      }      
	return false;
	  
  }
  public static boolean touchPort(String iname, int irate) throws SerialException {
    SerialPort port;
    boolean result = false;
    try {
      Enumeration portList = CommPortIdentifier.getPortIdentifiers();
      while (portList.hasMoreElements()) {
        CommPortIdentifier portId = (CommPortIdentifier) portList.nextElement();
        if ((CommPortIdentifier.PORT_SERIAL == portId.getPortType()) && (portId.getName().equals(iname))) {
          port = (SerialPort) portId.open("tap", 2000);
          port.setSerialPortParams(irate, 8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
          port.close();				
          result = true;
        }
      }
    } catch (PortInUseException e) {
      throw new SerialException(
        I18n.format(_("Serial port ''{0}'' already in use. Try quitting any programs that may be using it."), iname)
      );
    } catch (Exception e) {
      throw new SerialException(
        I18n.format(_("Error touching serial port ''{0}''."), iname), e
      );
    }
	return result;
  }

  public Serial(String iname, int irate,
                 char iparity, int idatabits, float istopbits)
  throws SerialException {
    //if (port != null) port.close();
    //this.parent = parent;
    //parent.attach(this);

    this.rate = irate;
    
    parity = SerialPort.PARITY_NONE;
    if (iparity == 'E') parity = SerialPort.PARITY_EVEN;
    if (iparity == 'O') parity = SerialPort.PARITY_ODD;

    this.databits = idatabits;

    stopbits = SerialPort.STOPBITS_1;
    if (istopbits == 1.5f) stopbits = SerialPort.STOPBITS_1_5;
    if (istopbits == 2) stopbits = SerialPort.STOPBITS_2;

    try {
      port = null;
      Enumeration portList = CommPortIdentifier.getPortIdentifiers();
      while (portList.hasMoreElements()) {
        CommPortIdentifier portId =
          (CommPortIdentifier) portList.nextElement();

        if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
          //System.out.println("found " + portId.getName());
          if (portId.getName().equals(iname)) {
            //System.out.println("looking for "+iname);
            port = (SerialPort)portId.open("CM9SerialDownloader", 2000); //[ROBOTIS] 2013-06-11 changed application Owner name
          //portId.addPortOwnershipListener(this); //[ROBOTIS] add to manage ownership of serial port
            input = port.getInputStream();
            output = port.getOutputStream();
            port.setSerialPortParams(rate, databits, stopbits, parity);
            port.addEventListener(this);
            port.notifyOnDataAvailable(true);
            port.notifyOnBreakInterrupt(true);//[ROBOTIS] although it does not work in Windows, added to support BI event.
            // port.notifyOnCarrierDetect(true);
            //port.notifyOnFramingError(true);
           // port.notifyOnOverrunError(true);
            //port.notifyOnParityError(true);
            
            //port.setFlowControlMode(SerialPort.FLOWCONTROL_XONXOFF_IN | SerialPort.FLOWCONTROL_XONXOFF_OUT);
            //port.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | SerialPort.FLOWCONTROL_RTSCTS_OUT);
            port.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);            
           //System.out.println("[Serial]opening, ready to roll");
            /*System.out.println("opening "+iname+" rate : "+rate+" databits : "+databits+" stopbits : "+stopbits+" parity : "+parity
            		+" Own Name = "+portId.isCurrentlyOwned()+" Own = "+portId.getCurrentOwner());*/
          }
        }
      }
    } catch (PortInUseException e) {
      throw new SerialException(
        I18n.format(
          _("Serial port ''{0}'' already in use. Try quiting any programs that may be using it."),
          iname
        )
      );
    } catch (Exception e) {
      throw new SerialException(
        I18n.format(
          _("Error opening serial port ''{0}''."),
          iname
        ),
        e
      );
//      //errorMessage("<init>", e);
//      //exception = e;
//      //e.printStackTrace();
    }
    
    if (port == null) {
    	System.out.println("[ROBOTIS CM9] Port is missing, unplug USB cable and re-start this program");
    }
  }

  public void ownershipChange(int type) {
	    switch (type) {
	      case CommPortOwnershipListener.PORT_UNOWNED:
	        System.out.println(Preferences.get("serial.port") + ": PORT_UNOWNED");
	        break;
	      case CommPortOwnershipListener.PORT_OWNED:
	        System.out.println(Preferences.get("serial.port") + ": PORT_OWNED");
	        break;
	      case CommPortOwnershipListener.PORT_OWNERSHIP_REQUESTED:
	        System.out.println(Preferences.get("serial.port") + ": PORT_INUSED");
	        this.dispose();
	        break;
	    }
 }
  public void sendDtrNegativeEdge(){
	if(port != null){
	
		//port.setDTR(false);
		port.setRTS(true);
		
			 try {
				Thread.sleep(100);
			} catch (InterruptedException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		 //port.setDTR(true);
		port.setRTS(false);
		 //dispose();
		
	}
	  
  }
  public void setup() {
    //parent.registerCall(this, DISPOSE);
  }


  //public void size(int w, int h) { }

  //public void pre() { }

  //public void draw() { }

  //public void post() { }

  //public void mouse(java.awt.event.MouseEvent event) { }

  //public void key(java.awt.event.KeyEvent e) { }


  public void dispose() {
    try {
      // do io streams need to be closed first?
      if (input != null) input.close();
      if (output != null) output.close();

    } catch (Exception e) {
      e.printStackTrace();
    }
    input = null;
    output = null;

    try {
      if (port != null) port.close();  // close the port

      //System.out.println("[Serial]port is closed!");
    } catch (Exception e) {
      e.printStackTrace();
    }
    //portId.removePortOwnershipListener(this);//add to manage ownership of serial port
    port = null;
  }
  
  
  public void addListener(MessageConsumer consumer) {
    this.consumer = consumer;
  }

  
  synchronized public void serialEvent(SerialPortEvent serialEvent) {
    //System.out.println("serial port event"); // " + serialEvent);
    //System.out.flush();
    //System.out.println("into");
    //System.out.flush();
    //System.err.println("type " + serialEvent.getEventType());
    //System.err.println("ahoooyey");
    //System.err.println("ahoooyeysdfsdfsdf");
    if (serialEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
      //System.out.println("data available");
      //System.err.flush();
      try {
        while (input.available() > 0) {
        //if (input.available() > 0) {
          //serial = input.read();
          //serialEvent();
          //buffer[bufferCount++] = (byte) serial;
          synchronized (buffer) {
            if (bufferLast == buffer.length) {
              byte temp[] = new byte[bufferLast << 1];
              System.arraycopy(buffer, 0, temp, 0, bufferLast);
              buffer = temp;
            }
            //buffer[bufferLast++] = (byte) input.read();
            /*if(monitor == true)
              System.out.print((char) input.read());*/
         
            /*
             * [ROBOTIS][CHANGE][START] we changed download code unlike Arduino and Maple.
             * We don't use external utilitiy such as Avrdude or dfutils.
             * just using serial communication via RX/TX library, send a binary to the CM9 serise board.
             * 
             * 
             * */
            else if(pandoraBoard == true){
               byte rxByteData = (byte) input.read();
	           if(rxByteData == (byte)0xa){
	        	   
	        	   buffer[bufferLast]= '\0';
	        	   String serialReply = new String(buffer).trim();
	        	   
	        	   //System.out.println("Enter Detected -> buffer is = "+serialReply);
	        	   if(serialReply.equals(downloadReady)){
	        		   //System.out.println("[Serial]Download Ready... ");
	        		   robotisSharedObject.setBoardIsExist(); //notify ready of download to SO(shared object)
	        		   DownloadBinaryViaSerial();
	        		 }
	        	   else if(serialReply.equals(downloadSuccess)){
	        		   //System.out.println("[Serial] Download is success!");
	        		   
	        		   robotisSharedObject.barkingDog(true);
	       		   
	        	   }
	        	   else if(serialReply.equals(downloadFail)){
	        		   //proxyEditorForThreadControl.unlockExportThread();
	        		 
	        		   //System.out.println("[Serial] Download is fail -> try one more");
	        		   robotisSharedObject.barkingDog(false);
	        		   
	        		  
	        	   }
	        	   //System.out.println("bufferLast = "+ Integer.toString(bufferLast) );
	        	   Arrays.fill(buffer, (byte)'\0');		        	   
	        	   bufferLast = 0;
		       }else{
	        	   buffer[bufferLast++] = rxByteData;// (byte) input.read();
		           //System.out.format("0x%x ", rxByteData);   
	        	   
	           }
            }
            //[ROBOTIS][CHANGE][END] to support pandora cm-900 board
            if (this.consumer != null)
              this.consumer.message("" + (char) input.read());
            
            /*
            System.err.println(input.available() + " " + 
                               ((char) buffer[bufferLast-1]));
            */            //}
          }
        }
        //System.out.println("no more");

      } catch (IOException e) {
        errorMessage("serialEvent", e);
        //e.printStackTrace();
        //System.out.println("angry");
      }
      catch (Exception e) {
      }
    }else if(serialEvent.getEventType() == SerialPortEvent.BI){
		System.out.println("[ROBOTIS] Break line detected!!!");
		dispose();
	}
    //System.out.println("out of");
    //System.err.println("out of event " + serialEvent.getEventType());
  }
//[ROBOTIS][CHANGE][START] to support pandora cm-900 board
  /*
  * [ROBOTIS][CHANGE] we changed download code unlike Arduino and Maple.
  * We don't use external utilitiy such as Avrdude or dfutils.
  * just using serial communication via RX/TX library, send a binary to the CM9 serise board.
  * 
  * 
  * */
  String binaryPathFromSketch = null;
  private DataInputStream binaryInput;
  private byte checkSumByte=0;
 
  public void DownloadBinaryViaSerial(){
		// TODO Auto-generated method stub
		//System.out.println("Download running...");
		byte[] buff = new byte[2048];
		
		int size;
		
		
		try {
			binaryInput = new DataInputStream(new FileInputStream(binaryPathFromSketch));
		} catch (FileNotFoundException e1) {
			// TODO Auto-generated catch block
			System.out.println("File not found : "+binaryPathFromSketch);
			e1.printStackTrace();
		}
		
		//System.out.println("Start uploading");
		try {
			while((size = binaryInput.read(buff)) > -1){
				output.write(buff, 0, size);
				
				//fileSize += size;
				/*for(int i=0 ; i < size ; i++){
					checkSumByte += buff[i];
				}*/
			
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			binaryInput.close();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	   /* File file = new File(binaryPathFromSketch);
	    System.out.println("[ROBOTIS]binary size : "+file.length()+"Bytes");*/
		checkSumByte = ArmCompiler.getChecksumOfBinary(); //[ROBOTIS]2012-12-19 change method to calculate checksum for mac OS x
		String outTemp = String.format("0x%x", checkSumByte);
		//System.out.println("Checksum : "+outTemp);
		//SerialWrite(checkSumByte);
		port.setDTR(true);
		write(checkSumByte);
		
		try {
			binaryInput.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
  }

  public byte GetCheckSumByte(){
	  return checkSumByte;
  }
  public void SetBinaryPath(String tmp){
	  binaryPathFromSketch = tmp;
  }
  public InputStream getSerialInputStream(){	
	return input ;	  
  }
  public OutputStream getSerialOutputStream(){	
	return output ;	  
  }
  public boolean IsConnectedSerial(){
	  if ( port != null)
		  return true;
	  return false;	  
  }
//[ROBOTIS][CHANGE][END] to support pandora cm-900 board

  /**
   * Returns the number of bytes that have been read from serial
   * and are waiting to be dealt with by the user.
   */
  public int available() {
    return (bufferLast - bufferIndex);
  }


  /**
   * Ignore all the bytes read so far and empty the buffer.
   */
  public void clear() {
    bufferLast = 0;
    bufferIndex = 0;
  }


  /**
   * Returns a number between 0 and 255 for the next byte that's 
   * waiting in the buffer. 
   * Returns -1 if there was no byte (although the user should
   * first check available() to see if things are ready to avoid this)
   */
  public int read() {
    if (bufferIndex == bufferLast) return -1;

    synchronized (buffer) {
      int outgoing = buffer[bufferIndex++] & 0xff;
      if (bufferIndex == bufferLast) {  // rewind
        bufferIndex = 0;
        bufferLast = 0;
      }
      return outgoing;
    }
  }


  /**
   * Returns the next byte in the buffer as a char.
   * Returns -1, or 0xffff, if nothing is there.
   */
  public char readChar() {
    if (bufferIndex == bufferLast) return (char)(-1);
    return (char) read();
  }


  /**
   * Return a byte array of anything that's in the serial buffer.
   * Not particularly memory/speed efficient, because it creates
   * a byte array on each read, but it's easier to use than 
   * readBytes(byte b[]) (see below).
   */
  public byte[] readBytes() {
    if (bufferIndex == bufferLast) return null;

    synchronized (buffer) {
      int length = bufferLast - bufferIndex;
      byte outgoing[] = new byte[length];
      System.arraycopy(buffer, bufferIndex, outgoing, 0, length);

      bufferIndex = 0;  // rewind
      bufferLast = 0;
      return outgoing;
    }
  }


  /**
   * Grab whatever is in the serial buffer, and stuff it into a 
   * byte buffer passed in by the user. This is more memory/time 
   * efficient than readBytes() returning a byte[] array. 
   *
   * Returns an int for how many bytes were read. If more bytes
   * are available than can fit into the byte array, only those
   * that will fit are read.
   */
  public int readBytes(byte outgoing[]) {
    if (bufferIndex == bufferLast) return 0;

    synchronized (buffer) {
      int length = bufferLast - bufferIndex;
      if (length > outgoing.length) length = outgoing.length;
      System.arraycopy(buffer, bufferIndex, outgoing, 0, length);

      bufferIndex += length;
      if (bufferIndex == bufferLast) {
        bufferIndex = 0;  // rewind
        bufferLast = 0;
      }
      return length;
    }    
  }


  /**
   * Reads from the serial port into a buffer of bytes up to and
   * including a particular character. If the character isn't in 
   * the serial buffer, then 'null' is returned.
   */
  public byte[] readBytesUntil(int interesting) {
    if (bufferIndex == bufferLast) return null;
    byte what = (byte)interesting;

    synchronized (buffer) {
      int found = -1; 
      for (int k = bufferIndex; k < bufferLast; k++) {
        if (buffer[k] == what) {
          found = k;
          break;
        }
      }
      if (found == -1) return null;

      int length = found - bufferIndex + 1;
      byte outgoing[] = new byte[length];
      System.arraycopy(buffer, bufferIndex, outgoing, 0, length);

      bufferIndex = 0;  // rewind
      bufferLast = 0;
      return outgoing;
    }
  }


  /**
   * Reads from the serial port into a buffer of bytes until a 
   * particular character. If the character isn't in the serial
   * buffer, then 'null' is returned.
   *
   * If outgoing[] is not big enough, then -1 is returned, 
   *   and an error message is printed on the console.
   * If nothing is in the buffer, zero is returned.
   * If 'interesting' byte is not in the buffer, then 0 is returned.
   */
  public int readBytesUntil(int interesting, byte outgoing[]) {
    if (bufferIndex == bufferLast) return 0;
    byte what = (byte)interesting;

    synchronized (buffer) {
      int found = -1; 
      for (int k = bufferIndex; k < bufferLast; k++) {
        if (buffer[k] == what) {
          found = k;
          break;
        }
      }
      if (found == -1) return 0;

      int length = found - bufferIndex + 1;
      if (length > outgoing.length) {
        System.err.println(
          I18n.format(
            _("readBytesUntil() byte buffer is too small for the {0}" +
              " bytes up to and including char {1}"),
            length,
            interesting
          )
        );
        return -1;
      }
      //byte outgoing[] = new byte[length];
      System.arraycopy(buffer, bufferIndex, outgoing, 0, length);

      bufferIndex += length;
      if (bufferIndex == bufferLast) {
        bufferIndex = 0;  // rewind
        bufferLast = 0;
      }
      return length;
    }
  }


  /**
   * Return whatever has been read from the serial port so far
   * as a String. It assumes that the incoming characters are ASCII. 
   *
   * If you want to move Unicode data, you can first convert the
   * String to a byte stream in the representation of your choice
   * (i.e. UTF8 or two-byte Unicode data), and send it as a byte array.
   */
  public String readString() {
    if (bufferIndex == bufferLast) return null;
    return new String(readBytes());
  }


  /**
   * Combination of readBytesUntil and readString. See caveats in 
   * each function. Returns null if it still hasn't found what
   * you're looking for.
   *
   * If you want to move Unicode data, you can first convert the
   * String to a byte stream in the representation of your choice
   * (i.e. UTF8 or two-byte Unicode data), and send it as a byte array.
   */
  public String readStringUntil(int interesting) {
    byte b[] = readBytesUntil(interesting);
    if (b == null) return null;
    return new String(b);
  }


  /**
   * This will handle both ints, bytes and chars transparently.
   */
  public void write(int what) {  // will also cover char
    try {
      output.write(what & 0xff);  // for good measure do the &
      output.flush();   // hmm, not sure if a good idea

    } catch (Exception e) { // null pointer or serial port dead
      errorMessage("write", e);
    }
  }


  public void write(byte bytes[]) {
    try {
      output.write(bytes);
      output.flush();   // hmm, not sure if a good idea

    } catch (Exception e) { // null pointer or serial port dead
      //errorMessage("write", e);
      e.printStackTrace();
    }
  }


  /**
   * Write a String to the output. Note that this doesn't account
   * for Unicode (two bytes per char), nor will it send UTF8 
   * characters.. It assumes that you mean to send a byte buffer 
   * (most often the case for networking and serial i/o) and
   * will only use the bottom 8 bits of each char in the string.
   * (Meaning that internally it uses String.getBytes)
   *
   * If you want to move Unicode data, you can first convert the
   * String to a byte stream in the representation of your choice
   * (i.e. UTF8 or two-byte Unicode data), and send it as a byte array.
   */
  public void write(String what) {
    write(what.getBytes());
  }

  public void setDTR(boolean state) {
    port.setDTR(state);
  }

  public void setRTS(boolean state) {
    port.setRTS(state);
  }

  /**
   * If this just hangs and never completes on Windows, 
   * it may be because the DLL doesn't have its exec bit set.
   * Why the hell that'd be the case, who knows.
   */
  static public List<String> list() {
    List<String> list = new ArrayList<String>();
    try {
      //System.err.println("trying");
      @SuppressWarnings("unchecked")
      Enumeration portList = CommPortIdentifier.getPortIdentifiers();
      //System.err.println("got port list");
      while (portList.hasMoreElements()) {
        CommPortIdentifier portId = 
          (CommPortIdentifier) portList.nextElement();
        //System.out.println(portId);

        if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
          String name = portId.getName();
          list.add(name);
        }
      }

    } catch (UnsatisfiedLinkError e) {
      //System.err.println("1");
      errorMessage("ports", e);

    } catch (Exception e) {
      //System.err.println("2");
      errorMessage("ports", e);
    }
    //System.err.println("move out");
    return list;
  }


  /**
   * General error reporting, all corraled here just in case
   * I think of something slightly more intelligent to do.
   */
  static public void errorMessage(String where, Throwable e) {
    System.err.println(I18n.format(_("Error inside Serial.{0}()"), where));
    e.printStackTrace();
  }
}


  /*
  class SerialMenuListener implements ItemListener {
    //public SerialMenuListener() { }

    public void itemStateChanged(ItemEvent e) {
      int count = serialMenu.getItemCount();
      for (int i = 0; i < count; i++) {
        ((CheckboxMenuItem)serialMenu.getItem(i)).setState(false);
      }
      CheckboxMenuItem item = (CheckboxMenuItem)e.getSource();
      item.setState(true);
      String name = item.getLabel();
      //System.out.println(item.getLabel());
      PdeBase.properties.put("serial.port", name);
      //System.out.println("set to " + get("serial.port"));
    }
  }
  */


  /*
  protected Vector buildPortList() {
    // get list of names for serial ports
    // have the default port checked (if present)
    Vector list = new Vector();

    //SerialMenuListener listener = new SerialMenuListener();
    boolean problem = false;

    // if this is failing, it may be because
    // lib/javax.comm.properties is missing.
    // java is weird about how it searches for java.comm.properties
    // so it tends to be very fragile. i.e. quotes in the CLASSPATH
    // environment variable will hose things.
    try {
      //System.out.println("building port list");
      Enumeration portList = CommPortIdentifier.getPortIdentifiers();
      while (portList.hasMoreElements()) {
        CommPortIdentifier portId = 
          (CommPortIdentifier) portList.nextElement();
        //System.out.println(portId);

        if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
          //if (portId.getName().equals(port)) {
          String name = portId.getName();
          //CheckboxMenuItem mi = 
          //new CheckboxMenuItem(name, name.equals(defaultName));

          //mi.addItemListener(listener);
          //serialMenu.add(mi);
          list.addElement(name);
        }
      }
    } catch (UnsatisfiedLinkError e) {
      e.printStackTrace();
      problem = true;

    } catch (Exception e) {
      System.out.println("exception building serial menu");
      e.printStackTrace();
    }

    //if (serialMenu.getItemCount() == 0) {
      //System.out.println("dimming serial menu");
    //serialMenu.setEnabled(false);
    //}

    // only warn them if this is the first time
    if (problem && PdeBase.firstTime) {
      JOptionPane.showMessageDialog(this, //frame,
                                    "Serial port support not installed.\n" +
                                    "Check the readme for instructions\n" +
                                    "if you need to use the serial port.    ",
                                    "Serial Port Warning",
                                    JOptionPane.WARNING_MESSAGE);
    }
    return list;
  }
  */


 
