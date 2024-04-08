package javaComm;

public class Sender {  // Save as HelloJNI.java

	int sendCnt=0;
	
	static Sender sender;
	
	private Sender() {}
	
	synchronized  static Sender getInstance() {
		if (sender==null)
			sender=new Sender();
		return sender;
	}
	
	public static int send(String str) {
		getInstance().sendCnt++;
//	   System.out.println("java send(): " + str);
	   return 1;	// always succeed
	}
   
	public static int getSendCnt() {
		return getInstance().sendCnt;
	}
	
}
