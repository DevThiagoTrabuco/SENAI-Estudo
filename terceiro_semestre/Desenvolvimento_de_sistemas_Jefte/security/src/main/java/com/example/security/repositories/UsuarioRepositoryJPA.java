package com.example.security.repositories;

import org.springframework.data.jpa.repository.JpaRepository;
import com.example.security.entities.Usuario;

public interface UsuarioRepositoryJPA extends JpaRepository<Usuario, Integer> {

}
