import java.util.*;
import java.lang.StringBuilder;

class HelloWorld 
{
    public static void main(String[] args) 
    {
	System.out.println("Hello World!"); 
	System.out.println(args[0]);
	StringTokenizer tokenizer = new StringTokenizer(args[0]);
	
	while (tokenizer.hasMoreTokens())
	    System.out.println(tokenizer.nextToken());

	String hello = "hello";
	String world = "world";

	System.out.println(hello + ", " + world);

	Integer i = Integer.valueOf(args[1]);
	System.out.println(i + 5);

	String istring = i.toString();
	System.out.println(istring + " is a string.");

	StringBuilder message = new StringBuilder(hello);
	message.append(", ");
	message.append(world);

	System.out.println(message);
    }
}

