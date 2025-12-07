package HotelAPI.repository;


import HotelAPI.model.Usuario;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface UsuarioRepository extends JpaRepository<Usuario, Integer> {

    List<Usuario>  findByNome(String nome);
}
