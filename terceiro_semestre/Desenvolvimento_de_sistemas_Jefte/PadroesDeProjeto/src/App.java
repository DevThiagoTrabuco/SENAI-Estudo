public class App {
    public static void main(String[] args) throws Exception {
        PessoaFactory factory = new PessoaFactory();
        Pessoa pessoa1 = factory.criarPessoa("João", 25);
        Pessoa pessoa2 = factory.criarPessoa("Maria", 30);
        Pessoa pessoa3 = factory.criarPessoa("José", 40);
        System.out.println(pessoa1.getId() + " " + pessoa1.getNome() + " " + pessoa1.getIdade());
        System.out.println(pessoa2.getId() + " " + pessoa2.getNome() + " " + pessoa2.getIdade());
        System.out.println(pessoa3.getId() + " " + pessoa3.getNome() + " " + pessoa3.getIdade());
    }
}
