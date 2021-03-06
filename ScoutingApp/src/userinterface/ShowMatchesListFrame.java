package userinterface;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.border.TitledBorder;

import teams.Team;
import matches.Match;
import matches.MatchesList;

public class ShowMatchesListFrame extends JPanel{
	
	MatchesList list = new MatchesList();
	JScrollPane scrollPane = new JScrollPane(list.table);
	
	public ShowMatchesListFrame(){
		list.table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		scrollPane.setBorder(new TitledBorder("Matches"));
		add(scrollPane);
		
		
	}

	public static void main(String[] args) {
		Team[] teamsArray = new Team[4];
		teamsArray[Match.RED_1] = new Team(5029, "Powerstackers");
		teamsArray[Match.RED_2] = new Team(4251, "Cougar Robotics");
		teamsArray[Match.BLUE_1] = new Team(5501, "USS Enterprise");
		teamsArray[Match.BLUE_2] = new Team(5035, "Some random team");
		
		
		
		Match match1 = new Match(teamsArray, Match.MATCHTYPE_QUALIFICATION, 1);
		match1.setRedScore(50);
		Match match2 = new Match(teamsArray, Match.MATCHTYPE_QUALIFICATION, 2);
		match2.setRedScore(60);
		match2.setBlueScore(67);
		
		ShowMatchesListFrame frame = new ShowMatchesListFrame();
		frame.list.addMatch(match1);
		frame.list.addMatch(match2);
		
		JFrame framee = new JFrame();
		framee.add(frame);
		framee.setSize(500, 500);
		framee.setLocationRelativeTo(null);
		framee.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		framee.setVisible(true);

		

	}

}
