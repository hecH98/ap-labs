import java.util.LinkedList;

import javax.swing.JOptionPane;

public class FirstCodeChallenge {
	private String chain;
	private LinkedList<Character> answer;

	public FirstCodeChallenge(String chain) {
		this.chain = chain;
		this.answer = analizeString();
		System.out.println("The longest string length is: " + this.answer.size());
		System.out.println("The longest string is: " + this.answer);
	}

	private LinkedList<Character> analizeString() {
		int lastRepeated = 0;
		LinkedList<Character> list = new LinkedList<Character>();
		LinkedList<Character> answer = new LinkedList<Character>();
		for(int i = 0; i < this.chain.length(); i++) {
			if(!list.contains(this.chain.charAt(i))) {
				list.add(this.chain.charAt(i));
			}
			else {
				if(answer.size() < list.size()) {
					answer = list;
				}
				list = new LinkedList<Character>();
				i = lastRepeated;
				lastRepeated++;
			}
		}
		return answer;
	}

	public static void main(String[] args) {
		String chain = JOptionPane.showInputDialog("Input the string you want to analize");
		new FirstCodeChallenge(chain);
	}
}

