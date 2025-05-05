package com.example.security.application;

import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Component;

import com.example.security.entities.Usuario;
import com.example.security.repositories.UsuarioRepositoryJPA;

@Component
public class UsuarioApplication {
    private UsuarioRepositoryJPA usuarioRepositoryJPA;
    BCryptPasswordEncoder bCryptPasswordEncoder = new BCryptPasswordEncoder();

    public UsuarioApplication(UsuarioRepositoryJPA usuarioRepositoryJPA) {
        this.usuarioRepositoryJPA = usuarioRepositoryJPA;
    }

    public void cadastrarUsuario(Usuario usuario) {
        usuario.setSenha("{bcrypt}" + bCryptPasswordEncoder.encode(usuario.getSenha()));
        usuarioRepositoryJPA.save(usuario);
    }
}
