package processing.app;

public class SharedObjectForThreadControl {

	Serial proxy;
	boolean downloadResult = false;
	boolean checkBoardExist = false;
	void setSerialProxy(Serial tmp){
		
		proxy = tmp;
	}
	void setBoardIsExist(){
		checkBoardExist = true;
	}
	boolean getBoardIsExist(){
		return checkBoardExist;
	}
	synchronized void barkingDog(boolean result) { 
	 
		//System.out.println("[ROBOTIS]Now, notify end of download"); 
		downloadResult = result;
		notify(); 
		 
	} 
	synchronized boolean sleepingBaby() { 
		
		try {
			
			Thread thread = new Thread(new Runnable() {
			    @Override
			    public void run() {
			    	downloadTimoutChecker();
			    }
			});
			thread.start();			
			//System.out.println("[BABY]Waiting for finishing downloading...");
			wait(); 
		} catch(InterruptedException e) { 
			System.out.println("InterruptedException caught"); 
		} 
		 
		//System.out.println("[ROBOTIS]Wake up!");
		return downloadResult; 
		 
	}
	synchronized void downloadTimoutChecker() {
		checkBoardExist = false;
	    long endTimeMillis = System.currentTimeMillis() + 5000;
	    while (true) {
	        // method logic
	    	if(checkBoardExist == true){
	    		//System.out.println("[DOG] Board is okay -> go on downloading");
	    		return;
	    	}
	        if (System.currentTimeMillis() > endTimeMillis) {
	            // do some clean-up
	        	//System.out.println("[DOG] Board is not responsed");
	        	notify();
	            return;
	        }
	    }
	}
}
