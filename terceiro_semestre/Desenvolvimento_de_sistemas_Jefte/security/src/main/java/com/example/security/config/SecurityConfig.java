package com.example.security.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.Customizer;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.provisioning.InMemoryUserDetailsManager;
import org.springframework.security.web.SecurityFilterChain;

@Configuration
public class SecurityConfig {
    @Bean
    public InMemoryUserDetailsManager userDetailsManager() {
        UserDetails brenno = User.builder()
                .username("brenno")
                .password("{noop}master@123")
                .roles("ADMIN", "USUARIO")
                .build();

        UserDetails barbara = User.builder()
                .username("barbara")
                .password("{noop}master@123")
                .roles("ADMIN", "USUARIO")
                .build();

        UserDetails jefte = User.builder()
                .username("jefte")
                .password("{noop}master@123")
                .roles("USUARIO")
                .build();

        return new InMemoryUserDetailsManager(brenno, barbara, jefte);
    }

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity httpSecurity) throws Exception {
        httpSecurity.authorizeHttpRequests(configurer -> {
            configurer
                    .requestMatchers("/api/admin/**").hasRole("ADMIN")
                    .requestMatchers("/api/admin/**").hasRole("ADMIN")
                    .requestMatchers( "/api/user/**").hasRole("USUARIO")
                    .requestMatchers("/api/usuario/**").permitAll();
        });

        httpSecurity.httpBasic(Customizer.withDefaults());

        httpSecurity.csrf(csrf -> csrf.disable());

        return httpSecurity.build();
    }
}
