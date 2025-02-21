package edu.java.lab01;

public class stepik {
    private String printTextPerRole(String[] roles, String[] textLines) {
        String text="";
        for(int i=0; i<roles.length; i++){
            text = text+roles[i]+":\n";
            for(int k=0; k<textLines.length; k++){
                if (textLines[k].startsWith(roles[i]+": ")){
                    text=text+(k+1)+") "+textLines[k].replace(roles[i]+": ","")+"\n";
                }
            }
            text=text+"\n";
        }
        return text;
    }
}
