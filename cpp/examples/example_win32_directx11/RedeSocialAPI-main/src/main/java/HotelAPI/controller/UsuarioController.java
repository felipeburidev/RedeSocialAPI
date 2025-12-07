package HotelAPI.controller;

import HotelAPI.model.Usuario;
import HotelAPI.repository.UsuarioRepository;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/usuarios")
@CrossOrigin("*")
public class UsuarioController {

    private final UsuarioRepository usuarioRepository;

    public UsuarioController(UsuarioRepository usuarioRepository) {
        this.usuarioRepository = usuarioRepository;
    }

    //Get Todos os usuarios
    @GetMapping("/get")
    public List<Usuario> findAll() {
        return usuarioRepository.findAll();
    }

    //Get por ID
    @GetMapping("/id/{id}")
    public Usuario findById(@PathVariable int id) {
        return usuarioRepository.findById(id).orElse(null);
    }

    //Post
    @PostMapping("/post")
    public Usuario save(@RequestBody Usuario usuario) {
        return usuarioRepository.save(usuario);
    }

    //Buscar por nome
    @GetMapping
    public List<Usuario> buscar(@RequestParam(required = false) String nome) {
            return usuarioRepository.findByNome(nome);
    }

    //Deletar Usuario por id
    @DeleteMapping("/dell{id}")
    public void delete(@PathVariable int id) {
        usuarioRepository.deleteById(id);
    }

    @PutMapping("/alter{id}")
    public Usuario update(@RequestBody Usuario usuario, @PathVariable int id) {
        return usuarioRepository.save(usuario);
    }


}
