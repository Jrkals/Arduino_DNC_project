import java.util.HashMap;
import java.util.Random;

public class ArduinoProtocolMain {

	static HashMap<Integer, String> intsToBase4Map = new HashMap<>();
	static HashMap<Integer, Character> lightSignals = new HashMap<>();
	static HashMap<Character, Integer> lightSignalsFlip = new HashMap<>();
	static HashMap<String, Integer> base4Tobase10 = new HashMap<>();

	static final int RATE = 8; // rate of errors is 1/this number

	public static void main(String[] args) {
		//Initialize hashmaps
		lightSignals.put(0, 'b'); // 0 is black
		lightSignals.put(1, 'r'); // 1 is red
		lightSignals.put(2, 'g'); // 2 is green
		lightSignals.put(3, 'y'); // 3 is yellow
		lightSignals.put(4, 'w'); // 4 is white

		lightSignalsFlip.put('b', 0);
		lightSignalsFlip.put('r', 1);
		lightSignalsFlip.put('g', 2);
		lightSignalsFlip.put('y', 3);
		lightSignalsFlip.put('w', 4);

		base4Tobase10.put("1020", 72); // H
		base4Tobase10.put("1221", 105); // i
		base4Tobase10.put("200", 32); // ' '
		base4Tobase10.put("1310", 116); // t
		base4Tobase10.put("1220", 104); // h
		base4Tobase10.put("1211", 101); // e
		base4Tobase10.put("1302", 114); // r
		base4Tobase10.put("201", 33); // !

		intsToBase4Map.put(72, "1020"); // H
		intsToBase4Map.put(105, "1221"); // i
		intsToBase4Map.put(32, "200"); // ' '
		intsToBase4Map.put(116, "1310"); // t
		intsToBase4Map.put(104, "1220"); // h
		intsToBase4Map.put(101, "1211"); // e
		intsToBase4Map.put(114, "1302"); // r
		intsToBase4Map.put(33, "201"); // !

		System.out.print("original message is: ");
		String testString = "Hi there!"; // level 1
		System.out.println(testString);
		
		String[] asBaseFourString = makeBase4(testString); // level 2
		String[] encodedMessage = encode(asBaseFourString); // level 3 TODO
		String[] lightSignals = convertToLight(encodedMessage); // level 4
		
		System.out.print("Message as light signals is: ");
		printArray(lightSignals); // message
		
		//TODO INSERT errors here
		//System.out.println("message recieved as light signals is :");
		//String[] messageWithErrors = induceErrors(lightSignals); // transmission, level 4

		//Now to decode
		System.out.print("message decoded from light to numbers is: ");
		String[] lightSignalsAsNums = convertFromLight(lightSignals); // level 3
		printArray(lightSignalsAsNums);
		
		System.out.print("message decoded after error correction is: ");
		String[] decodedMessage = decode(lightSignalsAsNums); // level 2 TODO
		printArray(decodedMessage);
		
		System.out.print("message in base 10 is: ");
		String[] base10Message = makeBase10(decodedMessage); // level 1.5
		printArray(base10Message);
		
		String finalMessage = convertToChars(base10Message); // level 1

		System.out.print("final message is : ");
		System.out.println(finalMessage);

		if(finalMessage.equals(testString))
			System.out.println("SUCCESS");
		else
			System.out.println("FAILURE");

	}
	/*
	 * takes base 10 string and converts it to an array of chars
	 */
	private static String convertToChars(String[] base10Message) {
		String rv = "";
		for(int i = 0; i < base10Message.length; i++) {
			String temp = base10Message[i];
			int tempAsInt = Integer.parseInt(temp);
			char newChar = (char)tempAsInt;
			rv += newChar;
		}
		return rv;
	}
	/*
	 * TODO put error correcting bits and other things in
	 */
	private static String[] encode(String[] asBaseFourString) {
		// TODO Auto-generated method stub
		return asBaseFourString;
	}
	/*
	 * TODO decode the message and remove error correcting bits
	 */
	private static String[] decode(String[] lightSignalsAsNums) {
		// TODO Auto-generated method stub
		return lightSignalsAsNums;
	}
	/*
	 * takes an array of light signals and converts it to an array
	 * if base4 nums
	 * example: ["bbbbb", "bbrggr"]->["00000", "001221"]
	 */
	private static String[] convertFromLight(String[] messageWithErrors) {
		String[] rv = new String[messageWithErrors.length];
		for(int i = 0; i < messageWithErrors.length; i++) {
			String temp = messageWithErrors[i];
			String lightAsNums = "";
			for(int j = 0; j < temp.length(); j++) {
				lightAsNums += lightSignalsFlip.get(temp.charAt(j));
			}
			rv[i] = lightAsNums;
		}
		return rv;
	}
	

	/*
	 * takes an array of base four numbers and converts it to
	 * an array of light signals
	 * e.g., ["00110", "12332"] -> ["bbrrb", "rgyyg"];
	 */
	private static String[] convertToLight(String[] encodedMessage) {
		String[] rv = new String[encodedMessage.length];
		for(int i = 0; i < encodedMessage.length;i++) {
			String newLightSignal = ""; // entry into the array rv
			String temp = encodedMessage[i]; // current base 4 string
			for(int j = 0; j < temp.length(); j++) {
				//System.out.println(" and temp[j] is "+temp.charAt(j));
				int cv = Integer.parseInt(temp.charAt(j) + "");
				newLightSignal += lightSignals.get(cv); // get the char for the given num value
			}
			rv[i] = newLightSignal;
		}
		//printArray(rv);
		return rv;
	}
	
	/*
	 * takes an array of base 4 numbers and converts them into an array of
	 *  base10 strings
	 *  example: ["1020", "1221"]->["72","105"]
	 */
	private static String[] makeBase10(String[] decodedMessage) {
		String[] rv = new String[decodedMessage.length];
		for(int i = 0; i < decodedMessage.length; i++) {
			String temp = decodedMessage[i];
			String base10 = base4Tobase10.get(temp) + "";

			rv[i] = base10;
		}
		return rv;
	}
	

	/*
	 *  takes a string and returns it as a base 4 string array 
	 *  one entry per character
	 *  example: "Hi" returns ["1020","1221"]
	 */
	private static String[] makeBase4(String testString) {
		String[] rv = new String[testString.length()];
		for(int i = 0; i < testString.length(); i++) {
			char c = testString.charAt(i);
			rv[i] = intsToBase4Map.get((int)c);
		}
		return rv;
	}
	
	/*
	 * swap bits at a rate of 1/RATE
	 */
	private static String[] induceErrors(String[] lightSignals2) {
		String rv[] = new String[lightSignals2.length];
		for(int i = 0; i < lightSignals2.length; i++) {
			String temp = lightSignals2[i];
			for(int j = 0; j < temp.length(); j++) {
				int num = (int)Math.random()*RATE;
				if(num == 1) {
					temp = ""; //TODO do something to swap the char
				}
			}
		}
		return rv;
	}
	/*
	 * computes the hamming distance between two equal length strings
	 */
	static int findHammingDistance(String s1, String s2) {
		if(s1.length() != s2.length()) {
			return -1; // this should never happen
		}
		int distance = 0; // hamming distance
		for(int i = 0; i < s1.length(); i++) {
			if(s1.charAt(i) != s2.charAt(i)) {
				distance++;
			}
		}
		
		return distance;
	}

	// print a one dimensional string array
	static void printArray(String[] a) {
		for (int i = 0; i < a.length; i++) {
			System.out.print(a[i]+ "\t ");
		}
		System.out.println();
	}

}
