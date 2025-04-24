public class PessoaFactory {
    private int contadorId = 0;

    public Pessoa criarPessoa(String nome, int idade) {
        Pessoa pessoa = new Pessoa(contadorId, nome, idade);
        contadorId++;
        return pessoa;
    }
}
